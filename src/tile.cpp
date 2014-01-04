/*
Minetest-c55
Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "tile.h"
#include "debug.h"
#include "main.h" // for g_settings
#include "filesys.h"
#include "utility.h"
#include "settings.h"
#include "mesh.h"
#include <ICameraSceneNode.h>
#include "log.h"
#include "mapnode.h" // For texture atlas making
#include "mineral.h" // For texture atlas making
#include "path.h"

/*
	TextureSource
*/

TextureSource::TextureSource(IrrlichtDevice *device):
		m_device(device),
		m_main_atlas_image(NULL),
		m_main_atlas_texture(NULL)
{
	assert(m_device);

	m_atlaspointer_cache_mutex.Init();

	m_main_thread = get_current_thread_id();

	// Add a NULL AtlasPointer as the first index, named ""
	m_atlaspointer_cache.push_back(SourceAtlasPointer(""));
	m_name_to_id[""] = 0;

	// Build main texture atlas
	if(g_settings->getBool("enable_texture_atlas"))
		buildMainAtlas();
	else
		infostream<<"Not building texture atlas."<<std::endl;
}

TextureSource::~TextureSource()
{
}

void TextureSource::processQueue()
{
	/*
		Fetch textures
	*/
	if(m_get_texture_queue.size() > 0)
	{
		GetRequest<std::string, u32, u8, u8>
				request = m_get_texture_queue.pop();

		infostream<<"TextureSource::processQueue(): "
				<<"got texture request with "
				<<"name=\""<<request.key<<"\""
				<<std::endl;

		GetResult<std::string, u32, u8, u8>
				result;
		result.key = request.key;
		result.callers = request.callers;
		result.item = getTextureIdDirect(request.key);

		request.dest->push_back(result);
	}
}

u32 TextureSource::getTextureId(const std::string &name)
{
	//infostream<<"getTextureId(): \""<<name<<"\""<<std::endl;

	{
		/*
			See if texture already exists
		*/
		JMutexAutoLock lock(m_atlaspointer_cache_mutex);
		core::map<std::string, u32>::Node *n;
		n = m_name_to_id.find(name);
		if(n != NULL)
		{
			return n->getValue();
		}
	}

	/*
		Get texture
	*/
	if(get_current_thread_id() == m_main_thread)
	{
		return getTextureIdDirect(name);
	}
	else
	{
		infostream<<"getTextureId(): Queued: name=\""<<name<<"\""<<std::endl;

		// We're gonna ask the result to be put into here
		ResultQueue<std::string, u32, u8, u8> result_queue;

		// Throw a request in
		m_get_texture_queue.add(name, 0, 0, &result_queue);

		infostream<<"Waiting for texture from main thread, name=\""
				<<name<<"\""<<std::endl;

		try
		{
			// Wait result for a second
			GetResult<std::string, u32, u8, u8>
					result = result_queue.pop_front(1000);

			// Check that at least something worked OK
			assert(result.key == name);

			return result.item;
		}
		catch(ItemNotFoundException &e)
		{
			infostream<<"Waiting for texture timed out."<<std::endl;
			return 0;
		}
	}

	infostream<<"getTextureId(): Failed"<<std::endl;

	return 0;
}

// Draw a progress bar on the image
void make_progressbar(float value, video::IImage *image);

/*
	Generate image based on a string like "stone.png" or "[crack0".
	if baseimg is NULL, it is created. Otherwise stuff is made on it.
*/
bool generate_image(std::string part_of_name, video::IImage *& baseimg,
		IrrlichtDevice *device);

/*
	Generates an image from a full string like
	"stone.png^mineral_coal.png^[crack0".

	This is used by buildMainAtlas().
*/
video::IImage* generate_image_from_scratch(std::string name,
		IrrlichtDevice *device);

/*
	This method generates all the textures
*/
u32 TextureSource::getTextureIdDirect(const std::string &name)
{
	//infostream<<"getTextureIdDirect(): name=\""<<name<<"\""<<std::endl;

	// Empty name means texture 0
	if(name == "")
	{
		infostream<<"getTextureIdDirect(): name is empty"<<std::endl;
		return 0;
	}

	/*
		Calling only allowed from main thread
	*/
	if(get_current_thread_id() != m_main_thread)
	{
		errorstream<<"TextureSource::getTextureIdDirect() "
				"called not from main thread"<<std::endl;
		return 0;
	}

	/*
		See if texture already exists
	*/
	{
		JMutexAutoLock lock(m_atlaspointer_cache_mutex);

		core::map<std::string, u32>::Node *n;
		n = m_name_to_id.find(name);
		if(n != NULL)
		{
			infostream<<"getTextureIdDirect(): \""<<name
					<<"\" found in cache"<<std::endl;
			return n->getValue();
		}
	}

	infostream<<"getTextureIdDirect(): \""<<name
			<<"\" NOT found in cache. Creating it."<<std::endl;

	/*
		Get the base image
	*/

	char separator = '^';

	/*
		This is set to the id of the base image.
		If left 0, there is no base image and a completely new image
		is made.
	*/
	u32 base_image_id = 0;

	// Find last meta separator in name
	s32 last_separator_position = -1;
	for(s32 i=name.size()-1; i>=0; i--)
	{
		if(name[i] == separator)
		{
			last_separator_position = i;
			break;
		}
	}
	/*
		If separator was found, construct the base name and make the
		base image using a recursive call
	*/
	std::string base_image_name;
	if(last_separator_position != -1)
	{
		// Construct base name
		base_image_name = name.substr(0, last_separator_position);
		/*infostream<<"getTextureIdDirect(): Calling itself recursively"
				" to get base image of \""<<name<<"\" = \""
                <<base_image_name<<"\""<<std::endl;*/
		base_image_id = getTextureIdDirect(base_image_name);
	}

	//infostream<<"base_image_id="<<base_image_id<<std::endl;

	video::IVideoDriver* driver = m_device->getVideoDriver();
	assert(driver);

	video::ITexture *t = NULL;

	/*
		An image will be built from files and then converted into a texture.
	*/
	video::IImage *baseimg = NULL;

	// If a base image was found, copy it to baseimg
	if(base_image_id != 0)
	{
		JMutexAutoLock lock(m_atlaspointer_cache_mutex);

		SourceAtlasPointer ap = m_atlaspointer_cache[base_image_id];

		video::IImage *image = ap.atlas_img;

		if(image == NULL)
		{
			infostream<<"getTextureIdDirect(): NULL image in "
					<<"cache: \""<<base_image_name<<"\""
					<<std::endl;
		}
		else
		{
			core::dimension2d<u32> dim = ap.intsize;

			baseimg = driver->createImage(video::ECF_A8R8G8B8, dim);

			core::position2d<s32> pos_to(0,0);
			core::position2d<s32> pos_from = ap.intpos;

			image->copyTo(
					baseimg, // target
					v2s32(0,0), // position in target
					core::rect<s32>(pos_from, dim) // from
			);

			/*infostream<<"getTextureIdDirect(): Loaded \""
					<<base_image_name<<"\" from image cache"
					<<std::endl;*/
		}
	}

	/*
		Parse out the last part of the name of the image and act
		according to it
	*/

	std::string last_part_of_name = name.substr(last_separator_position+1);
	//infostream<<"last_part_of_name=\""<<last_part_of_name<<"\""<<std::endl;

	// Generate image according to part of name
	if(generate_image(last_part_of_name, baseimg, m_device) == false)
	{
		infostream<<"getTextureIdDirect(): "
				"failed to generate \""<<last_part_of_name<<"\""
				<<std::endl;
	}

	// If no resulting image, print a warning
	if(baseimg == NULL)
	{
		infostream<<"getTextureIdDirect(): baseimg is NULL (attempted to"
				" create texture \""<<name<<"\""<<std::endl;
	}

	if(baseimg != NULL)
	{
		// Create texture from resulting image
		t = driver->addTexture(name.c_str(), baseimg);
	}

	/*
		Add texture to caches (add NULL textures too)
	*/

	JMutexAutoLock lock(m_atlaspointer_cache_mutex);

	u32 id = m_atlaspointer_cache.size();
	AtlasPointer ap(id);
	ap.atlas = t;
	ap.pos = v2f(0,0);
	ap.size = v2f(1,1);
	ap.tiled = 0;
	core::dimension2d<u32> baseimg_dim(0,0);
	if(baseimg)
		baseimg_dim = baseimg->getDimension();
	SourceAtlasPointer nap(name, ap, baseimg, v2s32(0,0), baseimg_dim);
	m_atlaspointer_cache.push_back(nap);
	m_name_to_id.insert(name, id);

	/*infostream<<"getTextureIdDirect(): "
			<<"Returning id="<<id<<" for name \""<<name<<"\""<<std::endl;*/

	return id;
}

std::string TextureSource::getTextureName(u32 id)
{
	JMutexAutoLock lock(m_atlaspointer_cache_mutex);

	if(id >= m_atlaspointer_cache.size())
	{
		infostream<<"TextureSource::getTextureName(): id="<<id
				<<" >= m_atlaspointer_cache.size()="
				<<m_atlaspointer_cache.size()<<std::endl;
		return "";
	}

	return m_atlaspointer_cache[id].name;
}


AtlasPointer TextureSource::getTexture(u32 id)
{
	JMutexAutoLock lock(m_atlaspointer_cache_mutex);

	if(id >= m_atlaspointer_cache.size())
		return AtlasPointer(0, NULL);

	return m_atlaspointer_cache[id].a;
}

void TextureSource::buildMainAtlas()
{
	infostream<<"TextureSource::buildMainAtlas()"<<std::endl;

	//return; // Disable (for testing)

	video::IVideoDriver* driver = m_device->getVideoDriver();
	assert(driver);

	JMutexAutoLock lock(m_atlaspointer_cache_mutex);

	// Create an image of the right size
	core::dimension2d<u32> atlas_dim(2048,2048);
	core::dimension2d<u32> max_dim = driver->getMaxTextureSize();
	atlas_dim.Width  = MYMIN(atlas_dim.Width,  max_dim.Width);
	atlas_dim.Height = MYMIN(atlas_dim.Height, max_dim.Height);
	video::IImage *atlas_img =
			driver->createImage(video::ECF_A8R8G8B8, atlas_dim);
	//assert(atlas_img);
	if(atlas_img == NULL)
	{
		errorstream<<"TextureSource::buildMainAtlas(): Failed to create atlas "
				"image; not building texture atlas."<<std::endl;
		return;
	}

	/*
		Grab list of stuff to include in the texture atlas from the
		main content features
	*/

	core::map<std::string, bool> sourcelist;

	for(u16 j=0; j<MAX_CONTENT+1; j++)
	{
		if(j == CONTENT_IGNORE || j == CONTENT_AIR)
			continue;
		ContentFeatures *f = &content_features(j);
		for(core::map<std::string, bool>::Iterator
				i = f->used_texturenames.getIterator();
				i.atEnd() == false; i++)
		{
			std::string name = i.getNode()->getKey();
			sourcelist[name] = true;

			if(f->often_contains_mineral){
				for(int k=1; k<MINERAL_COUNT; k++){
					std::string mineraltexture = mineral_block_texture(k);
					std::string fulltexture = name + "^" + mineraltexture;
					sourcelist[fulltexture] = true;
				}
			}
		}
	}

	infostream<<"Creating texture atlas out of textures: ";
	for(core::map<std::string, bool>::Iterator
			i = sourcelist.getIterator();
			i.atEnd() == false; i++)
	{
		std::string name = i.getNode()->getKey();
		infostream<<"\""<<name<<"\" ";
	}
	infostream<<std::endl;

	// Padding to disallow texture bleeding
	s32 padding = 16;

	s32 column_width = 256;
	s32 column_padding = 16;

	/*
		First pass: generate almost everything
	*/
	core::position2d<s32> pos_in_atlas(0,0);

	pos_in_atlas.X = column_padding;
	pos_in_atlas.Y = padding;

	for(core::map<std::string, bool>::Iterator
			i = sourcelist.getIterator();
			i.atEnd() == false; i++)
	{
		std::string name = i.getNode()->getKey();

		/*video::IImage *img = driver->createImageFromFile(
				getTexturePath(name.c_str()).c_str());
		if(img == NULL)
			continue;

		core::dimension2d<u32> dim = img->getDimension();
		// Make a copy with the right color format
		video::IImage *img2 =
				driver->createImage(video::ECF_A8R8G8B8, dim);
		img->copyTo(img2);
		img->drop();*/

		// Generate image by name
		video::IImage *img2 = generate_image_from_scratch(name, m_device);
		if(img2 == NULL)
		{
			infostream<<"TextureSource::buildMainAtlas(): Couldn't generate texture atlas: Couldn't generate image \""<<name<<"\""<<std::endl;
			continue;
		}

		core::dimension2d<u32> dim = img2->getDimension();

		// Don't add to atlas if image is large
		core::dimension2d<u32> max_size_in_atlas(64,64);
		if(dim.Width > max_size_in_atlas.Width
		|| dim.Height > max_size_in_atlas.Height)
		{
			infostream<<"TextureSource::buildMainAtlas(): Not adding "
					<<"\""<<name<<"\" because image is large"<<std::endl;
			continue;
		}

		// Wrap columns and stop making atlas if atlas is full
		if(pos_in_atlas.Y + dim.Height > atlas_dim.Height)
		{
			if(pos_in_atlas.X > (s32)atlas_dim.Width - column_width - column_padding){
				errorstream<<"TextureSource::buildMainAtlas(): "
						<<"Atlas is full, not adding more textures."
						<<std::endl;
				break;
			}
			pos_in_atlas.Y = padding;
			pos_in_atlas.X += column_width + column_padding*2;
		}

        infostream<<"TextureSource::buildMainAtlas(): Adding \""<<name
                <<"\" to texture atlas"<<std::endl;

		// Tile it a few times in the X direction
		u16 xwise_tiling = column_width / dim.Width;
		if(xwise_tiling > 16) // Limit to 16 (more gives no benefit)
			xwise_tiling = 16;
		for(u32 j=0; j<xwise_tiling; j++)
		{
			// Copy the copy to the atlas
			//img2->copyToWithAlpha(atlas_img,
					//pos_in_atlas + v2s32(j*dim.Width,0),
					//core::rect<s32>(v2s32(0,0), dim),
					//video::SColor(255,255,255,255),
					//NULL);
			img2->copyTo(atlas_img,
					pos_in_atlas + v2s32(j*dim.Width,0),
					core::rect<s32>(v2s32(0,0), dim),
					NULL);
		}


		// Copy the borders a few times to disallow texture bleeding
		for(u32 side=0; side<2; side++) // top and bottom
		for(s32 y0=0; y0<padding; y0++)
		for(s32 x0=0; x0<(s32)xwise_tiling*(s32)dim.Width; x0++)
		{
			s32 dst_y;
			s32 src_y;
			if(side==0)
			{
				dst_y = y0 + pos_in_atlas.Y + dim.Height;
				src_y = pos_in_atlas.Y + dim.Height - 1;
			}
			else
			{
				dst_y = -y0 + pos_in_atlas.Y-1;
				src_y = pos_in_atlas.Y;
			}
			s32 x = x0 + pos_in_atlas.X;
			video::SColor c = atlas_img->getPixel(x, src_y);
			atlas_img->setPixel(x,dst_y,c);
		}

		for(u32 side=0; side<2; side++) // left and right
		for(s32 x0=0; x0<column_padding; x0++)
		for(s32 y0=-padding; y0<(s32)dim.Height+padding; y0++)
		{
			s32 dst_x;
			s32 src_x;
			if(side==0)
			{
				dst_x = x0 + pos_in_atlas.X + dim.Width*xwise_tiling;
				src_x = pos_in_atlas.X + dim.Width*xwise_tiling - 1;
			}
			else
			{
				dst_x = -x0 + pos_in_atlas.X-1;
				src_x = pos_in_atlas.X;
			}
			s32 y = y0 + pos_in_atlas.Y;
			s32 src_y = MYMAX(pos_in_atlas.Y, MYMIN(pos_in_atlas.Y + (s32)dim.Height - 1, y));
			s32 dst_y = y;
			video::SColor c = atlas_img->getPixel(src_x, src_y);
			atlas_img->setPixel(dst_x,dst_y,c);
		}

		img2->drop();

		/*
			Add texture to caches
		*/

		// Get next id
		u32 id = m_atlaspointer_cache.size();

		// Create AtlasPointer
		AtlasPointer ap(id);
		ap.atlas = NULL; // Set on the second pass
		ap.pos = v2f((float)pos_in_atlas.X/(float)atlas_dim.Width,
				(float)pos_in_atlas.Y/(float)atlas_dim.Height);
		ap.size = v2f((float)dim.Width/(float)atlas_dim.Width,
				(float)dim.Width/(float)atlas_dim.Height);
		ap.tiled = xwise_tiling;

		// Create SourceAtlasPointer and add to containers
		SourceAtlasPointer nap(name, ap, atlas_img, pos_in_atlas, dim);
		m_atlaspointer_cache.push_back(nap);
		m_name_to_id.insert(name, id);

		// Increment position
		pos_in_atlas.Y += dim.Height + padding * 2;
	}

	/*
		Make texture
	*/
	video::ITexture *t = driver->addTexture("__main_atlas__", atlas_img);
	assert(t);

	/*
		Second pass: set texture pointer in generated AtlasPointers
	*/
	for(core::map<std::string, bool>::Iterator
			i = sourcelist.getIterator();
			i.atEnd() == false; i++)
	{
		std::string name = i.getNode()->getKey();
		if(m_name_to_id.find(name) == NULL)
			continue;
		u32 id = m_name_to_id[name];
		//infostream<<"id of name "<<name<<" is "<<id<<std::endl;
		m_atlaspointer_cache[id].a.atlas = t;
	}

	/*
		Write image to file so that it can be inspected
	*/
	/*std::string atlaspath = porting::path_userdata
			+ DIR_DELIM + "generated_texture_atlas.png";
	infostream<<"Removing and writing texture atlas for inspection to "
			<<atlaspath<<std::endl;
	fs::RecursiveDelete(atlaspath);
	driver->writeImageToFile(atlas_img, atlaspath.c_str());*/
}

video::IImage* generate_image_from_scratch(std::string name,
		IrrlichtDevice *device)
{
	/*infostream<<"generate_image_from_scratch(): "
			"\""<<name<<"\""<<std::endl;*/

	video::IVideoDriver* driver = device->getVideoDriver();
	assert(driver);

	/*
		Get the base image
	*/

	video::IImage *baseimg = NULL;

	char separator = '^';

	// Find last meta separator in name
	s32 last_separator_position = -1;
	for(s32 i=name.size()-1; i>=0; i--)
	{
		if(name[i] == separator)
		{
			last_separator_position = i;
			break;
		}
	}

	/*infostream<<"generate_image_from_scratch(): "
			<<"last_separator_position="<<last_separator_position
			<<std::endl;*/

	/*
		If separator was found, construct the base name and make the
		base image using a recursive call
	*/
	std::string base_image_name;
	if(last_separator_position != -1)
	{
		// Construct base name
		base_image_name = name.substr(0, last_separator_position);
		/*infostream<<"generate_image_from_scratch(): Calling itself recursively"
				" to get base image of \""<<name<<"\" = \""
                <<base_image_name<<"\""<<std::endl;*/
		baseimg = generate_image_from_scratch(base_image_name, device);
	}

	/*
		Parse out the last part of the name of the image and act
		according to it
	*/

	std::string last_part_of_name = name.substr(last_separator_position+1);
	//infostream<<"last_part_of_name=\""<<last_part_of_name<<"\""<<std::endl;

	// Generate image according to part of name
	if(generate_image(last_part_of_name, baseimg, device) == false)
	{
		infostream<<"generate_image_from_scratch(): "
				"failed to generate \""<<last_part_of_name<<"\""
				<<std::endl;
		return NULL;
	}

	return baseimg;
}

bool generate_image(std::string part_of_name, video::IImage *& baseimg,
		IrrlichtDevice *device)
{
	video::IVideoDriver* driver = device->getVideoDriver();
	assert(driver);

	// Stuff starting with [ are special commands
	if(part_of_name[0] != '[')
	{
		// A normal texture; load it from a file
		std::string path = getTexturePath(part_of_name.c_str());
		/*infostream<<"generate_image(): Loading path \""<<path
				<<"\""<<std::endl;*/

		video::IImage *image = driver->createImageFromFile(path.c_str());

		if(image == NULL)
		{
			if (part_of_name != "") {
				infostream<<"generate_image(): Could not load image \""
			    <<part_of_name<<"\" from path \""<<path<<"\""
						<<" while building texture"<<std::endl;

				infostream<<"generate_image(): Creating a dummy"
			    <<" image for \""<<part_of_name<<"\""<<std::endl;
			}

			// Just create a dummy image
			core::dimension2d<u32> dim(1,1);
			image = driver->createImage(video::ECF_A8R8G8B8, dim);
			assert(image);
			image->setPixel(0,0, video::SColor(255,myrand()%256,
					myrand()%256,myrand()%256));
		}

		// If base image is NULL, load as base.
		if(baseimg == NULL)
		{
			//infostream<<"Setting "<<part_of_name<<" as base"<<std::endl;
			/*
				Copy it this way to get an alpha channel.
				Otherwise images with alpha cannot be blitted on
				images that don't have alpha in the original file.
			*/
			core::dimension2d<u32> dim = image->getDimension();
			baseimg = driver->createImage(video::ECF_A8R8G8B8, dim);
			image->copyTo(baseimg);
			image->drop();
		}
		// Else blit on base.
		else
		{
			//infostream<<"Blitting "<<part_of_name<<" on base"<<std::endl;
			// Size of the copied area
			core::dimension2d<u32> dim = image->getDimension();
			//core::dimension2d<u32> dim(16,16);
			// Position to copy the blitted to in the base image
			core::position2d<s32> pos_to(0,0);
			// Position to copy the blitted from in the blitted image
			core::position2d<s32> pos_from(0,0);
			// Blit
			image->copyToWithAlpha(baseimg, pos_to,
					core::rect<s32>(pos_from, dim),
					video::SColor(255,255,255,255),
					NULL);
			// Drop image
			image->drop();
		}
	}
	else
	{
		// A special texture modification

		infostream<<"generate_image(): generating special "
				<<"modification \""<<part_of_name<<"\""
				<<std::endl;

		/*
			This is the simplest of all; it just adds stuff to the
			name so that a separate texture is created.

			It is used to make textures for stuff that doesn't want
			to implement getting the texture from a bigger texture
			atlas.
		*/
		if(part_of_name == "[forcesingle")
		{
		}
		/*
			[crackN
			Adds a cracking texture
		*/
		else if(part_of_name.substr(0,6) == "[crack")
		{
			if(baseimg == NULL)
			{
				infostream<<"generate_image(): baseimg==NULL "
						<<"for part_of_name=\""<<part_of_name
						<<"\", cancelling."<<std::endl;
				return false;
			}

			// Crack image number
			u16 progression = stoi(part_of_name.substr(6));

			// Size of the base image
			core::dimension2d<u32> dim_base = baseimg->getDimension();

			/*
				Load crack image.

				It is an image with a number of cracking stages
				horizontally tiled.
			*/
			video::IImage *img_crack = driver->createImageFromFile(
					getTexturePath("crack.png").c_str());

			if(img_crack)
			{
				// Dimension of original image
				core::dimension2d<u32> dim_crack
						= img_crack->getDimension();
				// Count of crack stages
				u32 crack_count = dim_crack.Height / dim_crack.Width;
				// Limit progression
				if(progression > crack_count-1)
					progression = crack_count-1;
				// Dimension of a single scaled crack stage
				core::dimension2d<u32> dim_crack_scaled_single(
					dim_base.Width,
					dim_base.Height
				);
				// Dimension of scaled size
				core::dimension2d<u32> dim_crack_scaled(
					dim_crack_scaled_single.Width,
					dim_crack_scaled_single.Height * crack_count
				);
				// Create scaled crack image
				video::IImage *img_crack_scaled = driver->createImage(
						video::ECF_A8R8G8B8, dim_crack_scaled);
				if(img_crack_scaled)
				{
					// Scale crack image by copying
					img_crack->copyToScaling(img_crack_scaled);

					// Position to copy the crack from
					core::position2d<s32> pos_crack_scaled(
						0,
						dim_crack_scaled_single.Height * progression
					);

					// This tiling does nothing currently but is useful
					for(u32 y0=0; y0<dim_base.Height
							/ dim_crack_scaled_single.Height; y0++)
					for(u32 x0=0; x0<dim_base.Width
							/ dim_crack_scaled_single.Width; x0++)
					{
						// Position to copy the crack to in the base image
						core::position2d<s32> pos_base(
							x0*dim_crack_scaled_single.Width,
							y0*dim_crack_scaled_single.Height
						);
						// Rectangle to copy the crack from on the scaled image
						core::rect<s32> rect_crack_scaled(
							pos_crack_scaled,
							dim_crack_scaled_single
						);
						// Copy it
						img_crack_scaled->copyToWithAlpha(baseimg, pos_base,
								rect_crack_scaled,
								video::SColor(255,255,255,255),
								NULL);
					}

					img_crack_scaled->drop();
				}

				img_crack->drop();
			}
		}
		/*
			[combine:WxH:X,Y=filename:X,Y=filename2
			Creates a bigger texture from an amount of smaller ones
		*/
		else if(part_of_name.substr(0,8) == "[combine")
		{
			Strfnd sf(part_of_name);
			sf.next(":");
			u32 w0 = stoi(sf.next("x"));
			u32 h0 = stoi(sf.next(":"));
			infostream<<"combined w="<<w0<<" h="<<h0<<std::endl;
			core::dimension2d<u32> dim(w0,h0);
			baseimg = driver->createImage(video::ECF_A8R8G8B8, dim);
			while(sf.atend() == false)
			{
				u32 x = stoi(sf.next(","));
				u32 y = stoi(sf.next("="));
				std::string filename = sf.next(":");
				infostream<<"Adding \""<<filename
						<<"\" to combined ("<<x<<","<<y<<")"
						<<std::endl;
				video::IImage *img = driver->createImageFromFile(
						getTexturePath(filename.c_str()).c_str());
				if(img)
				{
					core::dimension2d<u32> dim = img->getDimension();
					infostream<<"Size "<<dim.Width
							<<"x"<<dim.Height<<std::endl;
					core::position2d<s32> pos_base(x, y);
					video::IImage *img2 =
							driver->createImage(video::ECF_A8R8G8B8, dim);
					img->copyTo(img2);
					img->drop();
					img2->copyToWithAlpha(baseimg, pos_base,
							core::rect<s32>(v2s32(0,0), dim),
							video::SColor(255,255,255,255),
							NULL);
					img2->drop();
				}
				else
				{
					infostream<<"img==NULL"<<std::endl;
				}
			}
		}
		/*
			[progressbarN
			Adds a progress bar, 0.0 <= N <= 1.0
		*/
		else if(part_of_name.substr(0,12) == "[progressbar")
		{
			if(baseimg == NULL)
			{
				infostream<<"generate_image(): baseimg==NULL "
						<<"for part_of_name=\""<<part_of_name
						<<"\", cancelling."<<std::endl;
				return false;
			}

			float value = stof(part_of_name.substr(12));
			make_progressbar(value, baseimg);
		}
		/*
			"[noalpha:filename.png"
			Use an image without it's alpha channel.
			Used for the leaves texture when in old leaves mode, so
			that the transparent parts don't look completely black
			when simple alpha channel is used for rendering.
		*/
		else if(part_of_name.substr(0,8) == "[noalpha")
		{
			if(baseimg != NULL)
			{
				infostream<<"generate_image(): baseimg!=NULL "
						<<"for part_of_name=\""<<part_of_name
						<<"\", cancelling."<<std::endl;
				return false;
			}

			std::string filename = part_of_name.substr(9);

			std::string path = getTexturePath(filename.c_str());

			infostream<<"generate_image(): Loading path \""<<path
					<<"\""<<std::endl;

			video::IImage *image = driver->createImageFromFile(path.c_str());

			if(image == NULL)
			{
				infostream<<"generate_image(): Loading path \""
						<<path<<"\" failed"<<std::endl;
			}
			else
			{
				core::dimension2d<u32> dim = image->getDimension();
				baseimg = driver->createImage(video::ECF_A8R8G8B8, dim);

				// Set alpha to full
				for(u32 y=0; y<dim.Height; y++)
				for(u32 x=0; x<dim.Width; x++)
				{
					video::SColor c = image->getPixel(x,y);
					c.setAlpha(255);
					image->setPixel(x,y,c);
				}
				// Blit
				image->copyTo(baseimg);

				image->drop();
			}
		}
		/*
			"[makealpha:R,G,B:filename.png"
			Use an image with converting one color to transparent.
		*/
		else if(part_of_name.substr(0,11) == "[makealpha:")
		{
			if(baseimg != NULL)
			{
				infostream<<"generate_image(): baseimg!=NULL "
						<<"for part_of_name=\""<<part_of_name
						<<"\", cancelling."<<std::endl;
				return false;
			}

			Strfnd sf(part_of_name.substr(11));
			u32 r1 = stoi(sf.next(","));
			u32 g1 = stoi(sf.next(","));
			u32 b1 = stoi(sf.next(":"));
			std::string filename = sf.next("");

			std::string path = getTexturePath(filename.c_str());

			infostream<<"generate_image(): Loading path \""<<path
					<<"\""<<std::endl;

			video::IImage *image = driver->createImageFromFile(path.c_str());

			if(image == NULL)
			{
				infostream<<"generate_image(): Loading path \""
						<<path<<"\" failed"<<std::endl;
			}
			else
			{
				core::dimension2d<u32> dim = image->getDimension();
				baseimg = driver->createImage(video::ECF_A8R8G8B8, dim);

				// Blit
				image->copyTo(baseimg);

				image->drop();

				for(u32 y=0; y<dim.Height; y++)
				for(u32 x=0; x<dim.Width; x++)
				{
					video::SColor c = baseimg->getPixel(x,y);
					u32 r = c.getRed();
					u32 g = c.getGreen();
					u32 b = c.getBlue();
					if(!(r == r1 && g == g1 && b == b1))
						continue;
					c.setAlpha(0);
					baseimg->setPixel(x,y,c);
				}
			}
		}
		/*
			"[makealpha2:R,G,B;R2,G2,B2:filename.png"
			Use an image with converting two colors to transparent.
		*/
		else if(part_of_name.substr(0,12) == "[makealpha2:")
		{
			if(baseimg != NULL)
			{
				infostream<<"generate_image(): baseimg!=NULL "
						<<"for part_of_name=\""<<part_of_name
						<<"\", cancelling."<<std::endl;
				return false;
			}

			Strfnd sf(part_of_name.substr(12));
			u32 r1 = stoi(sf.next(","));
			u32 g1 = stoi(sf.next(","));
			u32 b1 = stoi(sf.next(";"));
			u32 r2 = stoi(sf.next(","));
			u32 g2 = stoi(sf.next(","));
			u32 b2 = stoi(sf.next(":"));
			std::string filename = sf.next("");

			std::string path = getTexturePath(filename.c_str());

			infostream<<"generate_image(): Loading path \""<<path
					<<"\""<<std::endl;

			video::IImage *image = driver->createImageFromFile(path.c_str());

			if(image == NULL)
			{
				infostream<<"generate_image(): Loading path \""
						<<path<<"\" failed"<<std::endl;
			}
			else
			{
				core::dimension2d<u32> dim = image->getDimension();
				baseimg = driver->createImage(video::ECF_A8R8G8B8, dim);

				// Blit
				image->copyTo(baseimg);

				image->drop();

				for(u32 y=0; y<dim.Height; y++)
				for(u32 x=0; x<dim.Width; x++)
				{
					video::SColor c = baseimg->getPixel(x,y);
					u32 r = c.getRed();
					u32 g = c.getGreen();
					u32 b = c.getBlue();
					if(!(r == r1 && g == g1 && b == b1) &&
					   !(r == r2 && g == g2 && b == b2))
						continue;
					c.setAlpha(0);
					baseimg->setPixel(x,y,c);
				}
			}
		}
		/*
			"[transformN"
			Rotates and/or flips the image.

			N can be a number (between 0 and 7) or a transform name.
			Rotations are counter-clockwise.
			0  I      identity
			1  R90    rotate by 90 degrees
			2  R180   rotate by 180 degrees
			3  R270   rotate by 270 degrees
			4  FX     flip X
			5  FXR90  flip X then rotate by 90 degrees
			6  FY     flip Y
			7  FYR90  flip Y then rotate by 90 degrees

			Note: Transform names can be concatenated to produce
			their product (applies the first then the second).
			The resulting transform will be equivalent to one of the
			eight existing ones, though (see: dihedral group).
		*/
		else if(part_of_name.substr(0,10) == "[transform")
		{
			if(baseimg == NULL)
			{
				errorstream<<"generate_image(): baseimg==NULL "
						<<"for part_of_name=\""<<part_of_name
						<<"\", cancelling."<<std::endl;
				return false;
			}

			u32 transform = parseImageTransform(part_of_name.substr(10));
			core::dimension2d<u32> dim = imageTransformDimension(
					transform, baseimg->getDimension());
			video::IImage *image = driver->createImage(
					baseimg->getColorFormat(), dim);
			assert(image);
			imageTransform(transform, baseimg, image);
			baseimg->drop();
			baseimg = image;
		}
		/*
			[inventorycube{topimage{leftimage{rightimage
			In every subimage, replace ^ with &.
			Create an "inventory cube".
			NOTE: This should be used only on its own.
			Example (a grass block (not actually used in game):
			"[inventorycube{grass.png{mud.png&grass_side.png{mud.png&grass_side.png"
		*/
		else if(part_of_name.substr(0,14) == "[inventorycube")
		{
			if(baseimg != NULL)
			{
				errorstream<<"generate_image(): baseimg!=NULL "
						<<"for part_of_name=\""<<part_of_name
						<<"\", cancelling."<<std::endl;
				return false;
			}

			str_replace_char(part_of_name, '&', '^');
			Strfnd sf(part_of_name);
			sf.next("{");
			std::string imagename_top = sf.next("{");
			std::string imagename_left = sf.next("{");
			std::string imagename_right = sf.next("{");

			// Generate images for the faces of the cube
			video::IImage *img_top = generate_image_from_scratch(
					imagename_top, device);
			video::IImage *img_left = generate_image_from_scratch(
					imagename_left, device);
			video::IImage *img_right = generate_image_from_scratch(
					imagename_right, device);
			assert(img_top && img_left && img_right);

			// Create textures from images
			video::ITexture *texture_top = driver->addTexture(
					(imagename_top + "__temp__").c_str(), img_top);
			video::ITexture *texture_left = driver->addTexture(
					(imagename_left + "__temp__").c_str(), img_left);
			video::ITexture *texture_right = driver->addTexture(
					(imagename_right + "__temp__").c_str(), img_right);
			assert(texture_top && texture_left && texture_right);

			// Drop images
			img_top->drop();
			img_left->drop();
			img_right->drop();

			/*
				Draw a cube mesh into a render target texture
			*/
			scene::IMesh* cube = createCubeMesh(v3f(1, 1, 1));
			setMeshColor(cube, video::SColor(255, 255, 255, 255));
			cube->getMeshBuffer(0)->getMaterial().setTexture(0, texture_top);
			cube->getMeshBuffer(1)->getMaterial().setTexture(0, texture_top);
			cube->getMeshBuffer(2)->getMaterial().setTexture(0, texture_right);
			cube->getMeshBuffer(3)->getMaterial().setTexture(0, texture_right);
			cube->getMeshBuffer(4)->getMaterial().setTexture(0, texture_left);
			cube->getMeshBuffer(5)->getMaterial().setTexture(0, texture_left);

			core::dimension2d<u32> dim(64,64);
			std::string rtt_texture_name = part_of_name + "_RTT";

			v3f camera_position(0, 1.0, -1.5);
			camera_position.rotateXZBy(45);
			v3f camera_lookat(0, 0, 0);
			core::CMatrix4<f32> camera_projection_matrix;
			// Set orthogonal projection
			camera_projection_matrix.buildProjectionMatrixOrthoLH(
					1.65, 1.65, 0, 100);

			video::SColorf ambient_light(0.2,0.2,0.2);
			v3f light_position(10, 100, -50);
			video::SColorf light_color(0.5,0.5,0.5);
			f32 light_radius = 1000;

			video::ITexture *rtt = generateTextureFromMesh(
					cube, device, dim, rtt_texture_name,
					camera_position,
					camera_lookat,
					camera_projection_matrix,
					ambient_light,
					light_position,
					light_color,
					light_radius);

			// Drop mesh
			cube->drop();

			// Free textures of images
			driver->removeTexture(texture_top);
			driver->removeTexture(texture_left);
			driver->removeTexture(texture_right);

			if(rtt == NULL)
			{
				baseimg = generate_image_from_scratch(
						imagename_top, device);
				return true;
			}

			// Create image of render target
			video::IImage *image = driver->createImage(rtt, v2s32(0,0), dim);
			assert(image);

			baseimg = driver->createImage(video::ECF_A8R8G8B8, dim);

			if(image)
			{
				image->copyTo(baseimg);
				image->drop();
			}
		}
		/*
			[inventorynode{nodeid{topimage{leftimage{rightimage
			In every subimage, replace ^ with &.
			Create an "inventory stair".
			NOTE: This should be used only on its own.
			Example (a grass stair (not actually used in game):
			"[inventorynode{2048{grass.png{mud.png&grass_side.png{mud.png&grass_side.png"
			TODO: not implemented, only creates a cube
		*/
		else if(part_of_name.substr(0,14) == "[inventorynode")
		{
			if(baseimg != NULL)
			{
				errorstream<<"generate_image(): baseimg!=NULL "
						<<"for part_of_name=\""<<part_of_name
						<<"\", cancelling."<<std::endl;
				return false;
			}

			str_replace_char(part_of_name, '&', '^');
			Strfnd sf(part_of_name);
			sf.next("{");
			std::string node_id = sf.next("{");
			std::string imagename_top = sf.next("{");
			std::string imagename_left = sf.next("{");
			std::string imagename_right = sf.next("{");

			content_t c = atoi(node_id.c_str());

			// Generate images for the faces of the cube
			video::IImage *img_top = generate_image_from_scratch(
					imagename_top, device);
			video::IImage *img_left = generate_image_from_scratch(
					imagename_left, device);
			video::IImage *img_right = generate_image_from_scratch(
					imagename_right, device);
			assert(img_top && img_left && img_right);

			// Create textures from images
			video::ITexture *texture_top = driver->addTexture(
					(imagename_top + "__temp__").c_str(), img_top);
			video::ITexture *texture_left = driver->addTexture(
					(imagename_left + "__temp__").c_str(), img_left);
			video::ITexture *texture_right = driver->addTexture(
					(imagename_right + "__temp__").c_str(), img_right);
			assert(texture_top && texture_left && texture_right);

			// Drop images
			img_top->drop();
			img_left->drop();
			img_right->drop();

			/*
				Draw a mesh into a render target texture
			*/
			scene::IMesh* cube = createNodeBoxMesh(content_features(c).nodeboxes,v3f(1, 1, 1));
			setMeshColor(cube, video::SColor(255, 255, 255, 255));
			for (u16 i=0; i < content_features(c).nodeboxes.size(); i++) {
				cube->getMeshBuffer((i*6)+0)->getMaterial().setTexture(0, texture_top);
				cube->getMeshBuffer((i*6)+1)->getMaterial().setTexture(0, texture_top);
				cube->getMeshBuffer((i*6)+2)->getMaterial().setTexture(0, texture_right);
				cube->getMeshBuffer((i*6)+3)->getMaterial().setTexture(0, texture_right);
				cube->getMeshBuffer((i*6)+4)->getMaterial().setTexture(0, texture_left);
				cube->getMeshBuffer((i*6)+5)->getMaterial().setTexture(0, texture_left);
			}

			core::dimension2d<u32> dim(64,64);
			std::string rtt_texture_name = part_of_name + "_RTT";

			v3f camera_position(0, 1.0, -1.5);
			camera_position.rotateXZBy(45);
			v3f camera_lookat(0, 0, 0);
			core::CMatrix4<f32> camera_projection_matrix;
			// Set orthogonal projection
			camera_projection_matrix.buildProjectionMatrixOrthoLH(
					1.65, 1.65, 0, 100);

			video::SColorf ambient_light(0.2,0.2,0.2);
			v3f light_position(10, 100, -50);
			video::SColorf light_color(0.5,0.5,0.5);
			f32 light_radius = 1000;

			video::ITexture *rtt = generateTextureFromMesh(
					cube, device, dim, rtt_texture_name,
					camera_position,
					camera_lookat,
					camera_projection_matrix,
					ambient_light,
					light_position,
					light_color,
					light_radius);

			// Drop mesh
			cube->drop();

			// Free textures of images
			driver->removeTexture(texture_top);
			driver->removeTexture(texture_left);
			driver->removeTexture(texture_right);

			if(rtt == NULL)
			{
				baseimg = generate_image_from_scratch(
						imagename_top, device);
				return true;
			}

			// Create image of render target
			video::IImage *image = driver->createImage(rtt, v2s32(0,0), dim);
			assert(image);

			baseimg = driver->createImage(video::ECF_A8R8G8B8, dim);

			if(image)
			{
				image->copyTo(baseimg);
				image->drop();
			}
		}
		else
		{
			infostream<<"generate_image(): Invalid "
					" modification: \""<<part_of_name<<"\""<<std::endl;
		}
	}

	return true;
}

void make_progressbar(float value, video::IImage *image)
{
	if(image == NULL)
		return;

	core::dimension2d<u32> size = image->getDimension();

	u32 barheight = size.Height/16;
	u32 barpad_x = size.Width/16;
	u32 barpad_y = size.Height/16;
	u32 barwidth = size.Width - barpad_x*2;
	v2u32 barpos(barpad_x, size.Height - barheight - barpad_y);

	u32 barvalue_i = (u32)(((float)barwidth * value) + 0.5);
	u32 barvalue_c[10] = {
		(u32)(((float)barwidth * 0.1)),
		(u32)(((float)barwidth * 0.2)),
		(u32)(((float)barwidth * 0.3)),
		(u32)(((float)barwidth * 0.4)),
		(u32)(((float)barwidth * 0.5)),
		(u32)(((float)barwidth * 0.6)),
		(u32)(((float)barwidth * 0.7)),
		(u32)(((float)barwidth * 0.8)),
		(u32)(((float)barwidth * 0.9)),
		(u32)(((float)barwidth * 1.0))
	};

	video::SColor active[10] = {
		video::SColor(255,255,0,0),
		video::SColor(255,255,40,0),
		video::SColor(255,255,80,0),
		video::SColor(255,255,110,0),
		video::SColor(255,255,120,0),
		video::SColor(255,255,140,0),
		video::SColor(255,255,160,0),
		video::SColor(255,170,180,0),
		video::SColor(255,50,200,0),
		video::SColor(255,0,255,0)
	};
	video::SColor inactive(255,0,0,0);
	for(u32 x0=0; x0<barwidth; x0++)
	{
		video::SColor *c;
		if (x0 < barvalue_i) {
			if (x0 < barvalue_c[0]) {
				c = &active[0];
			}else if (x0 < barvalue_c[1]) {
				c = &active[1];
			}else if (x0 < barvalue_c[2]) {
				c = &active[2];
			}else if (x0 < barvalue_c[3]) {
				c = &active[3];
			}else if (x0 < barvalue_c[4]) {
				c = &active[4];
			}else if (x0 < barvalue_c[5]) {
				c = &active[5];
			}else if (x0 < barvalue_c[6]) {
				c = &active[6];
			}else if (x0 < barvalue_c[7]) {
				c = &active[7];
			}else if (x0 < barvalue_c[8]) {
				c = &active[8];
			}else{
				c = &active[9];
			}
		}else{
			c = &inactive;
		}
		u32 x = x0 + barpos.X;
		for(u32 y=barpos.Y; y<barpos.Y+barheight; y++)
		{
			image->setPixel(x,y, *c);
		}
	}
}

u32 parseImageTransform(const std::string& s)
{
	int total_transform = 0;

	std::string transform_names[8];
	transform_names[0] = "i";
	transform_names[1] = "r90";
	transform_names[2] = "r180";
	transform_names[3] = "r270";
	transform_names[4] = "fx";
	transform_names[6] = "fy";

	std::size_t pos = 0;
	while(pos < s.size())
	{
		int transform = -1;
		for(int i = 0; i <= 7; ++i)
		{
			const std::string &name_i = transform_names[i];

			if(s[pos] == ('0' + i))
			{
				transform = i;
				pos++;
				break;
			}
			else if(!(name_i.empty()) &&
				lowercase(s.substr(pos, name_i.size())) == name_i)
			{
				transform = i;
				pos += name_i.size();
				break;
			}
		}
		if(transform < 0)
			break;

		// Multiply total_transform and transform in the group D4
		int new_total = 0;
		if(transform < 4)
			new_total = (transform + total_transform) % 4;
		else
			new_total = (transform - total_transform + 8) % 4;
		if((transform >= 4) ^ (total_transform >= 4))
			new_total += 4;

		total_transform = new_total;
	}
	return total_transform;
}

core::dimension2d<u32> imageTransformDimension(u32 transform, core::dimension2d<u32> dim)
{
	if(transform % 2 == 0)
		return dim;
	else
		return core::dimension2d<u32>(dim.Height, dim.Width);
}

void imageTransform(u32 transform, video::IImage *src, video::IImage *dst)
{
	if(src == NULL || dst == NULL)
		return;

	core::dimension2d<u32> srcdim = src->getDimension();
	core::dimension2d<u32> dstdim = dst->getDimension();

	assert(dstdim == imageTransformDimension(transform, srcdim));
	assert(transform >= 0 && transform <= 7);

	/*
		Compute the transformation from source coordinates (sx,sy)
		to destination coordinates (dx,dy).
	*/
	int sxn = 0;
	int syn = 2;
	if(transform == 0)         // identity
		sxn = 0, syn = 2;  //   sx = dx, sy = dy
	else if(transform == 1)    // rotate by 90 degrees ccw
		sxn = 3, syn = 0;  //   sx = (H-1) - dy, sy = dx
	else if(transform == 2)    // rotate by 180 degrees
		sxn = 1, syn = 3;  //   sx = (W-1) - dx, sy = (H-1) - dy
	else if(transform == 3)    // rotate by 270 degrees ccw
		sxn = 2, syn = 1;  //   sx = dy, sy = (W-1) - dx
	else if(transform == 4)    // flip x
		sxn = 1, syn = 2;  //   sx = (W-1) - dx, sy = dy
	else if(transform == 5)    // flip x then rotate by 90 degrees ccw
		sxn = 2, syn = 0;  //   sx = dy, sy = dx
	else if(transform == 6)    // flip y
		sxn = 0, syn = 3;  //   sx = dx, sy = (H-1) - dy
	else if(transform == 7)    // flip y then rotate by 90 degrees ccw
		sxn = 3, syn = 1;  //   sx = (H-1) - dy, sy = (W-1) - dx

	for(u32 dy=0; dy<dstdim.Height; dy++)
	for(u32 dx=0; dx<dstdim.Width; dx++)
	{
		u32 entries[4] = {dx, dstdim.Width-1-dx, dy, dstdim.Height-1-dy};
		u32 sx = entries[sxn];
		u32 sy = entries[syn];
		video::SColor c = src->getPixel(sx,sy);
		dst->setPixel(dx,dy,c);
	}
}

