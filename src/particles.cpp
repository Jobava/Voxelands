/************************************************************************
* Minetest-c55
* Copyright (C) 2012 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* particles.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2014 <lisa@ltmnet.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*
* License updated from GPLv2 or later to GPLv3 or later by Lisa Milne
* for Voxelands.
************************************************************************/

#include "particles.h"
#include "constants.h"
#include "debug.h"
#include "main.h" // For g_profiler and g_settings
#include "settings.h"
#include "tile.h"
#include "collision.h"
#include <stdlib.h>
#include "utility.h"
#include "light.h"
#include "environment.h"
#include "map.h"
#include "mapnode.h"

v3f random_v3f(v3f min, v3f max)
{
	return v3f(
		rand()/(float)RAND_MAX*(max.X-min.X)+min.X,
		rand()/(float)RAND_MAX*(max.Y-min.Y)+min.Y,
		rand()/(float)RAND_MAX*(max.Z-min.Z)+min.Z
	);
}

Particle::Particle(
	scene::ISceneManager* smgr,
	LocalPlayer *player,
	s32 id,
	v3f pos,
	v3f velocity,
	v3f acceleration,
	float expirationtime,
	float size,
	AtlasPointer *ap
):
	scene::ISceneNode(smgr->getRootSceneNode(), smgr, id)
{
	m_camera_offset = v3s16(0,0,0);

	// Texture
	m_material.setFlag(video::EMF_LIGHTING, false);
	m_material.setFlag(video::EMF_BACK_FACE_CULLING, false);
	m_material.setFlag(video::EMF_BILINEAR_FILTER, false);
	m_material.setFlag(video::EMF_FOG_ENABLE, true);
	m_material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	m_material.setTexture(0, ap->atlas);
	m_ap = ap;
	m_light = 0;

	// Particle related
	m_pos = pos;
	m_velocity = velocity;
	m_acceleration = acceleration;
	m_expiration = expirationtime;
	m_time = 0;
	m_player = player;
	m_size = size;

	// Irrlicht stuff (TODO)
	m_collisionbox = core::aabbox3d<f32>(-size/2,-size/2,-size/2,size/2,size/2,size/2);
	this->setAutomaticCulling(scene::EAC_OFF);
}

Particle::~Particle()
{
	delete m_ap;
}

void Particle::OnRegisterSceneNode()
{
	if (IsVisible) {
		SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);
		SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);
	}

	ISceneNode::OnRegisterSceneNode();
}

void Particle::render()
{
	// TODO: Render particles in front of water and the selectionbox

	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	driver->setMaterial(m_material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	u16 indices[] = {0,1,2, 2,3,0};
	driver->drawVertexPrimitiveList(m_vertices, 4, indices, 2, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
}

void Particle::step(float dtime, ClientEnvironment &env)
{
	core::aabbox3d<f32> box = m_collisionbox;
	v3f p_pos = m_pos*BS;
	v3f p_velocity = m_velocity*BS;
	v3f p_acceleration = m_acceleration*BS;
	collisionMoveSimple(
		&env.getMap(),
		BS*0.5,
		box,
		0,
		dtime,
		p_pos,
		p_velocity,
		p_acceleration
	);
	m_pos = p_pos/BS;
	m_velocity = p_velocity/BS;
	m_acceleration = p_acceleration/BS;
	m_time += dtime;

	// Update lighting
	u8 light = 0;
	try{
		v3s16 p = v3s16(
			floor(m_pos.X+0.5),
			floor(m_pos.Y+0.5),
			floor(m_pos.Z+0.5)
		);
		MapNode n = env.getClientMap().getNode(p);
		light = n.getLightBlend(env.getDayNightRatio());
	}
	catch(InvalidPositionException &e){
		light = blend_light(env.getDayNightRatio(), LIGHT_SUN, 0);
	}
	m_light = decode_light(light);

	video::SColor c(255, m_light, m_light, m_light);

	m_vertices[0] = video::S3DVertex(-m_size/2,-m_size/2,0, 0,0,0,
	c, m_ap->x0(), m_ap->y1());
	m_vertices[1] = video::S3DVertex(m_size/2,-m_size/2,0, 0,0,0,
	c, m_ap->x1(), m_ap->y1());
	m_vertices[2] = video::S3DVertex(m_size/2,m_size/2,0, 0,0,0,
	c, m_ap->x1(), m_ap->y0());
	m_vertices[3] = video::S3DVertex(-m_size/2,m_size/2,0, 0,0,0,
	c, m_ap->x0(), m_ap->y0());

	for (u16 i=0; i<4; i++) {
		m_vertices[i].Pos.rotateYZBy(m_player->getPitch());
		m_vertices[i].Pos.rotateXZBy(m_player->getYaw());
		m_box.addInternalPoint(m_vertices[i].Pos);
		m_vertices[i].Pos += m_pos*BS - intToFloat(m_camera_offset, BS);
	}
}

std::vector<Particle*> all_particles;
std::map<u32, ParticleSpawner*> all_particlespawners;
u32 particle_spawner_ids = 1;

void allparticles_step (float dtime, ClientEnvironment &env)
{
	for (std::vector<Particle*>::iterator i = all_particles.begin(); i != all_particles.end();) {
		if ((*i)->get_expired()) {
			(*i)->remove();
			delete *i;
			all_particles.erase(i);
		}else{
			(*i)->step(dtime, env);
			i++;
		}
	}
}

void addDiggingParticles(scene::ISceneManager* smgr, LocalPlayer *player, v3s16 pos, const TileSpec tiles[])
{
	for (u16 j=0; j<DIGGING_PARTICLES_AMOUNT; j++) {
		addNodeParticle(smgr, player, pos, tiles);
	}
}

void addPunchingParticles(scene::ISceneManager* smgr, LocalPlayer *player, v3s16 pos, const TileSpec tiles[])
{
	addNodeParticle(smgr, player, pos, tiles);
}

// add a particle of a node
// used by digging and punching particles
void addNodeParticle(scene::ISceneManager* smgr, LocalPlayer *player, v3s16 pos, const TileSpec tiles[])
{
	// Texture
	u8 texid = myrand_range(0,5);
	AtlasPointer *ap = new AtlasPointer(tiles[texid].texture);
	float size = rand()%64/512.;
	float visual_size = BS*size;
	float texsize = size*2;

	float x1 = ap->x1();
	float y1 = ap->y1();

	ap->size.X = (ap->x1() - ap->x0()) * texsize;
	ap->size.Y = (ap->x1() - ap->x0()) * texsize;

	ap->pos.X = ap->x0() + (x1 - ap->x0()) * ((rand()%64)/64.-texsize);
	ap->pos.Y = ap->y0() + (y1 - ap->y0()) * ((rand()%64)/64.-texsize);

	// Physics
	v3f velocity((rand()%100/50.-1)/1.5, rand()%100/35., (rand()%100/50.-1)/1.5);
	v3f acceleration(0,-9,0);
	v3f particlepos = v3f(
		(f32)pos.X+rand()%100/200.-0.25,
		(f32)pos.Y+rand()%100/200.-0.25,
		(f32)pos.Z+rand()%100/200.-0.25
	);

	Particle *particle = new Particle(
		smgr,
		player,
		0,
		particlepos,
		velocity,
		acceleration,
		rand()%100/100., // expiration time
		visual_size,
		ap);

	all_particles.push_back(particle);
}

/*
	ParticleSpawner
*/

ParticleSpawner::ParticleSpawner(
	scene::ISceneManager *smgr,
	LocalPlayer *player,
	u16 amount,
	float time,
	v3f minpos,
	v3f maxpos,
	v3f minvel,
	v3f maxvel,
	v3f minacc,
	v3f maxacc,
	float minexptime,
	float maxexptime,
	float minsize,
	float maxsize,
	bool collisiondetection,
	AtlasPointer *ap,
	u32 id
)
{
	m_smgr = smgr;
	m_player = player;
	m_amount = amount;
	m_spawntime = time;
	m_minpos = minpos;
	m_maxpos = maxpos;
	m_minvel = minvel;
	m_maxvel = maxvel;
	m_minacc = minacc;
	m_maxacc = maxacc;
	m_minexptime = minexptime;
	m_maxexptime = maxexptime;
	m_minsize = minsize;
	m_maxsize = maxsize;
	m_collisiondetection = collisiondetection;
	m_ap = ap;
	m_time = 0;

	for (u16 i = 0; i<=m_amount; i++) {
		float spawntime = (float)rand()/(float)RAND_MAX*m_spawntime;
		m_spawntimes.push_back(spawntime);
	}

	all_particlespawners.insert(std::pair<u32, ParticleSpawner*>(id, this));
}

ParticleSpawner::~ParticleSpawner() {}

void ParticleSpawner::step(float dtime, ClientEnvironment &env)
{
	m_time += dtime;

	if (m_spawntime != 0) { // Spawner exists for a predefined timespan
		for (std::vector<float>::iterator i = m_spawntimes.begin(); i != m_spawntimes.end();) {
			if ((*i) <= m_time && m_amount > 0) {
				m_amount--;

				v3f pos = random_v3f(m_minpos, m_maxpos);
				v3f vel = random_v3f(m_minvel, m_maxvel);
				v3f acc = random_v3f(m_minacc, m_maxacc);
				float exptime = rand()/(float)RAND_MAX
						*(m_maxexptime-m_minexptime)
						+m_minexptime;
				float size = rand()/(float)RAND_MAX
						*(m_maxsize-m_minsize)
						+m_minsize;

				new Particle(
					m_smgr,
					m_player,
					0,
					pos,
					vel,
					acc,
					exptime,
					size,
					m_ap
				);
				m_spawntimes.erase(i);
			}else{
				i++;
			}
		}
	}else{ // Spawner exists for an infinity timespan, spawn on a per-second base
		for (int i = 0; i <= m_amount; i++) {
			if (rand()/(float)RAND_MAX < dtime) {
				v3f pos = random_v3f(m_minpos, m_maxpos);
				v3f vel = random_v3f(m_minvel, m_maxvel);
				v3f acc = random_v3f(m_minacc, m_maxacc);
				float exptime = rand()/(float)RAND_MAX
						*(m_maxexptime-m_minexptime)
						+m_minexptime;
				float size = rand()/(float)RAND_MAX
						*(m_maxsize-m_minsize)
						+m_minsize;

				new Particle(
					m_smgr,
					m_player,
					0,
					pos,
					vel,
					acc,
					exptime,
					size,
					m_ap
				);
			}
		}
	}
}

void allparticlespawners_step (float dtime, ClientEnvironment &env)
{
	for (std::map<u32, ParticleSpawner*>::iterator i = all_particlespawners.begin(); i != all_particlespawners.end();) {
		if (i->second->get_expired()) {
			delete i->second;
			all_particlespawners.erase(i++);
		}else{
			i->second->step(dtime, env);
			i++;
		}
	}
}

void delete_particlespawner (u32 id)
{
	std::map<u32, ParticleSpawner*>::iterator i = all_particlespawners.find(id);
	if (i == all_particlespawners.end())
		return;

	delete i->second;
	all_particlespawners.erase(id);
}

u32 add_particlespawner(
	scene::ISceneManager *smgr,
	LocalPlayer *player,
	u16 amount,
	float time,
	v3f minpos,
	v3f maxpos,
	v3f minvel,
	v3f maxvel,
	v3f minacc,
	v3f maxacc,
	float minexptime,
	float maxexptime,
	float minsize,
	float maxsize,
	AtlasPointer *ap
)
{
	u32 id = particle_spawner_ids++;
	ParticleSpawner *p = new ParticleSpawner(
		smgr,
		player,
		amount,
		time,
		minpos,
		maxpos,
		minvel,
		maxvel,
		minacc,
		maxacc,
		minexptime,
		maxexptime,
		minsize,
		maxsize,
		false,
		ap,
		id
	);

	if (p == NULL)
		return 0;

	return id;
}

void clear_particles ()
{
	for (std::map<u32, ParticleSpawner*>::iterator i = all_particlespawners.begin(); i != all_particlespawners.end();)
	{
		delete i->second;
		all_particlespawners.erase(i++);
	}

	for (std::vector<Particle*>::iterator i = all_particles.begin(); i != all_particles.end();)
	{
		(*i)->remove();
		delete *i;
		all_particles.erase(i);
	}
}

void update_particles_camera_offset(v3s16 camera_offset)
{
	for (std::vector<Particle*>::iterator i = all_particles.begin(); i != all_particles.end(); i++) {
		(*i)->updateCameraOffset(camera_offset);
	}
}
