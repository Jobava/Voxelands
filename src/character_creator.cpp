/************************************************************************
* character_creator.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2013-2014 <lisa@ltmnet.com>
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

#include "character_creator.h"
#include "settings.h"
#include "defaultsettings.h"
#include "guiKeyChangeMenu.h"
#include "debug.h"
#include "serialization.h"
#include <string>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include "path.h"
#include "utility.h"
#include "gui_colours.h"

#include "gettext.h"

GUICharDefMenu::GUICharDefMenu(
	IrrlichtDevice* device,
	gui::IGUIEnvironment* env,
	gui::IGUIElement* parent, s32 id,
	IMenuManager *menumgr,
	IGameCallback *gamecallback
):
	GUIModalMenu(env, parent, id, menumgr),
	m_accepted(false),
	m_gamecallback(gamecallback)
{
	this->env = env;
	this->parent = parent;
	this->id = id;
	this->menumgr = menumgr;
	m_model_rotation = 90;

	fetchPlayerSkin();

	m_skintone_types["white"] = wgettext("White Skin");
	m_skintone_types["red"] = wgettext("Red Skin");
	m_skintone_types["green"] = wgettext("Green Skin");
	m_skintone_types["fair"] = wgettext("Fair Skin");
	m_skintone_types["tanned"] = wgettext("Tanned Skin");
	m_skintone_types["dark"] = wgettext("Dark Skin");
	m_skintone_types["black"] = wgettext("Black Skin");

	m_face_types["human"] = wgettext("Human Face");
	m_face_types["elven"] = wgettext("Elven Face");
	m_face_types["dwarven"] = wgettext("Dwarven Face");
	m_face_types["alien"] = wgettext("Alien Face");

	m_hairtone_types["white"] = wgettext("White Hair");
	m_hairtone_types["blue"] = wgettext("Blue Hair");
	m_hairtone_types["green"] = wgettext("Green Hair");
	m_hairtone_types["orange"] = wgettext("Orange Hair");
	m_hairtone_types["brown"] = wgettext("Brown Hair");
	m_hairtone_types["purple"] = wgettext("Purple Hair");
	m_hairtone_types["red"] = wgettext("Red Hair");
	m_hairtone_types["blonde"] = wgettext("Blonde Hair");
	m_hairtone_types["black"] = wgettext("Black Hair");

	m_hair_types["short"] = wgettext("Short Hair");
	m_hair_types["medium"] = wgettext("Medium Hair");
	m_hair_types["long"] = wgettext("Long Hair");
	m_hair_types["special"] = wgettext("Styled Hair");

	m_eyes_types["white"] = wgettext("White Eyes");
	m_eyes_types["blue"] = wgettext("Blue Eyes");
	m_eyes_types["green"] = wgettext("Green Eyes");
	m_eyes_types["orange"] = wgettext("Orange Eyes");
	m_eyes_types["brown"] = wgettext("Brown Eyes");
	m_eyes_types["purple"] = wgettext("Purple Eyes");
	m_eyes_types["red"] = wgettext("Red Eyes");
	m_eyes_types["yellow"] = wgettext("Yellow Eyes");
	m_eyes_types["black"] = wgettext("Black Eyes");

	m_shirt_types["white"] = wgettext("White T-Shirt");
	m_shirt_types["blue"] = wgettext("Blue T-Shirt");
	m_shirt_types["green"] = wgettext("Green T-Shirt");
	m_shirt_types["orange"] = wgettext("Orange T-Shirt");
	m_shirt_types["purple"] = wgettext("Purple T-Shirt");
	m_shirt_types["red"] = wgettext("Red T-Shirt");
	m_shirt_types["yellow"] = wgettext("Yellow T-Shirt");
	m_shirt_types["black"] = wgettext("Black T-Shirt");

	m_pants_types["white"] = wgettext("White Pants");
	m_pants_types["blue"] = wgettext("Blue Pants");
	m_pants_types["green"] = wgettext("Green Pants");
	m_pants_types["orange"] = wgettext("Orange Pants");
	m_pants_types["purple"] = wgettext("Purple Pants");
	m_pants_types["red"] = wgettext("Red Pants");
	m_pants_types["yellow"] = wgettext("Yellow Pants");
	m_pants_types["black"] = wgettext("Black Pants");

	m_shoes_types["leather"] = wgettext("Leather Shoes");
	m_shoes_types["fur"] = wgettext("Fur Shoes");
	m_shoes_types["canvas"] = wgettext("Canvas Shoes");

	scene::ISceneManager* smgr = device->getSceneManager();

	m_cameranode = smgr->addCameraSceneNode(smgr->getRootSceneNode());
	m_cameranode->setPosition(v3f(20,0,0));
	// *100.0 helps in large map coordinates
	m_cameranode->setTarget(v3f(0,0,0));

	// get a unique mesh so that the player model has it's own lighting
	scene::IAnimatedMesh* mesh = createModelMesh(smgr,"character.b3d",true);
	if (!mesh)
		return;

	m_model = smgr->addAnimatedMeshSceneNode(mesh,smgr->getRootSceneNode());

	if (m_model) {
		m_model->setFrameLoop(0,79);
		m_model->setScale(v3f(m_xz_scale,m_y_scale,m_xz_scale));
		m_model->setRotation(v3f(0,m_model_rotation,0));
		setMeshColor(m_model->getMesh(), video::SColor(255,255,255,255));

		// Set material flags and texture
		m_model->setMaterialTexture(0, getPlayerTexture());
		video::SMaterial& material = m_model->getMaterial(0);
		material.setFlag(video::EMF_LIGHTING, false);
		material.setFlag(video::EMF_BILINEAR_FILTER, false);
		material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

		m_model->setPosition(v3f(0,0,0));
	}
#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
	mesh->drop();
#endif
}

GUICharDefMenu::~GUICharDefMenu()
{
	removeChildren();
}

void GUICharDefMenu::removeChildren()
{
	const core::list<gui::IGUIElement*> &children = getChildren();
	core::list<gui::IGUIElement*> children_copy;
	for (core::list<gui::IGUIElement*>::ConstIterator i = children.begin(); i != children.end(); i++) {
		children_copy.push_back(*i);
	}
	for (core::list<gui::IGUIElement*>::Iterator i = children_copy.begin(); i != children_copy.end(); i++) {
		(*i)->remove();
	}
}

//gender:Yscale:XZscale:skintone:eyes:hairtone:hair:face:shirt-colour:pants-colour:shoe-type
//M:10:10:fair:blue:brown:medium:normal:green:blue:leather

void GUICharDefMenu::regenerateGui(v2u32 screensize)
{

	m_screensize = screensize;

	/*
		Remove stuff
	*/
	removeChildren();

	/*
		Calculate new sizes and positions
	*/
	v2s32 size(200, 500);

	core::rect<s32> rect(
			10,
			screensize.Y/2 - size.Y/2,
			screensize.X - 10,
			screensize.Y/2 + size.Y/2
	);

	v2s32 leftside(0, 0);
	v2s32 rightside(screensize.X - (size.X+10), 0);

	DesiredRect = rect;
	recalculateAbsolutePosition(false);

	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 180, 300);
		rect += leftside + v2s32(10, 220);
		gui::IGUIStaticText *t = Environment->addStaticText(
			wgettext(
				"Here you can create your default character,"
				" this is how other players will see you in-game."
				" When you join a new server you will start with"
				" the clothing you select here. Additional"
				" clothing, both protective and decorative,"
				" can be crafted in-game.\n"
				"You can return here from the main menu anytime to"
				" change your character."
			),
			rect,
			false,
			true,
			this,
			-1
		);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}

	{
		core::rect<s32> rect(0, 0, 200, 20);
		rect += rightside + v2s32(0, 20);
		gui::IGUIStaticText *t = Environment->addStaticText(wgettext("Create Your Character"), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	// gender
	{
		core::rect<s32> rect(0, 0, 80, 30);
		rect += rightside + v2s32(15, 50);
		Environment->addButton(rect, this, GUI_ID_CD_MALE_BUTTON, wgettext("Male"));
	}
	{
		core::rect<s32> rect(0, 0, 80, 30);
		rect += rightside + v2s32(105, 50);
		Environment->addButton(rect, this, GUI_ID_CD_FEMALE_BUTTON, wgettext("Female"));
	}

	// Yscale
	{
		core::rect<s32> rect(0, 0, 80, 30);
		rect += rightside + v2s32(15, 95);
		Environment->addButton(rect, this, GUI_ID_CD_TALLER_BUTTON, wgettext("Taller"));
	}
	{
		core::rect<s32> rect(0, 0, 80, 30);
		rect += rightside + v2s32(15, 130);
		Environment->addButton(rect, this, GUI_ID_CD_SHORTER_BUTTON, wgettext("Shorter"));
	}

	// XZscale
	{
		core::rect<s32> rect(0, 0, 80, 30);
		rect += rightside + v2s32(105, 95);
		Environment->addButton(rect, this, GUI_ID_CD_WIDER_BUTTON, wgettext("Wider"));
	}
	{
		core::rect<s32> rect(0, 0, 80, 30);
		rect += rightside + v2s32(105, 130);
		Environment->addButton(rect, this, GUI_ID_CD_THINNER_BUTTON, wgettext("Thinner"));
	}

	// skintone
	//185
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += rightside + v2s32(45, 170);
		gui::IGUIStaticText *t = Environment->addStaticText(m_skintone_types[m_parts["skintone"]].c_str(), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(15, 165);
		Environment->addButton(rect, this, GUI_ID_CD_SKINTONE_PREV_BUTTON, L"<<");
	}
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(155, 165);
		Environment->addButton(rect, this, GUI_ID_CD_SKINTONE_NEXT_BUTTON, L">>");
	}

	// face
	//220
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += rightside + v2s32(45, 205);
		gui::IGUIStaticText *t = Environment->addStaticText(m_face_types[m_parts["face"]].c_str(), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(15, 200);
		Environment->addButton(rect, this, GUI_ID_CD_FACE_PREV_BUTTON, L"<<");
	}
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(155, 200);
		Environment->addButton(rect, this, GUI_ID_CD_FACE_NEXT_BUTTON, L">>");
	}

	// hairtone
	//255
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += rightside + v2s32(45, 240);
		gui::IGUIStaticText *t = Environment->addStaticText(m_hairtone_types[m_parts["hairtone"]].c_str(), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(15, 235);
		Environment->addButton(rect, this, GUI_ID_CD_HAIRTONE_PREV_BUTTON, L"<<");
	}
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(155, 235);
		Environment->addButton(rect, this, GUI_ID_CD_HAIRTONE_NEXT_BUTTON, L">>");
	}

	// hair
	//290
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += rightside + v2s32(45, 275);
		gui::IGUIStaticText *t = Environment->addStaticText(m_hair_types[m_parts["hair"]].c_str(), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(15, 270);
		Environment->addButton(rect, this, GUI_ID_CD_HAIR_PREV_BUTTON, L"<<");
	}
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(155, 270);
		Environment->addButton(rect, this, GUI_ID_CD_HAIR_NEXT_BUTTON, L">>");
	}

	// eyes
	//325
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += rightside + v2s32(45, 315);
		gui::IGUIStaticText *t = Environment->addStaticText(m_eyes_types[m_parts["eyes"]].c_str(), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(15, 305);
		Environment->addButton(rect, this, GUI_ID_CD_EYES_PREV_BUTTON, L"<<");
	}
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(155, 305);
		Environment->addButton(rect, this, GUI_ID_CD_EYES_NEXT_BUTTON, L">>");
	}

	// t-shirt
	//370
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += rightside + v2s32(45, 355);
		gui::IGUIStaticText *t = Environment->addStaticText(m_shirt_types[m_parts["shirt"]].c_str(), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(15, 350);
		Environment->addButton(rect, this, GUI_ID_CD_SHIRT_PREV_BUTTON, L"<<");
	}
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(155, 350);
		Environment->addButton(rect, this, GUI_ID_CD_SHIRT_NEXT_BUTTON, L">>");
	}

	// pants
	//405
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += rightside + v2s32(45, 390);
		gui::IGUIStaticText *t = Environment->addStaticText(m_pants_types[m_parts["pants"]].c_str(), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(15, 385);
		Environment->addButton(rect, this, GUI_ID_CD_PANTS_PREV_BUTTON, L"<<");
	}
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(155, 385);
		Environment->addButton(rect, this, GUI_ID_CD_PANTS_NEXT_BUTTON, L">>");
	}

	// shoes
	//440
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 110, 20);
		rect += rightside + v2s32(45, 425);
		gui::IGUIStaticText *t = Environment->addStaticText(m_shoes_types[m_parts["shoes"]].c_str(), rect, false, true, this, -1);
		t->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);
	}
	changeCtype("C");
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(15, 420);
		Environment->addButton(rect, this, GUI_ID_CD_SHOES_PREV_BUTTON, L"<<");
	}
	{
		core::rect<s32> rect(0, 0, 30, 30);
		rect += rightside + v2s32(155, 420);
		Environment->addButton(rect, this, GUI_ID_CD_SHOES_NEXT_BUTTON, L">>");
	}

	// save/cancel
	{
		core::rect<s32> rect(0, 0, 80, 30);
		rect += rightside + v2s32(15, 460);
		Environment->addButton(rect, this, GUI_ID_CD_SAVE_BUTTON, wgettext("Done"));
	}
	{
		core::rect<s32> rect(0, 0, 80, 30);
		rect += rightside + v2s32(105, 460);
		Environment->addButton(rect, this, GUI_ID_CD_QUIT_BUTTON, wgettext("Cancel"));
	}
}

void GUICharDefMenu::drawMenu()
{
	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;

	video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rect<s32> left(
		AbsoluteRect.UpperLeftCorner.X,
		AbsoluteRect.UpperLeftCorner.Y,
		AbsoluteRect.UpperLeftCorner.X+200,
		AbsoluteRect.LowerRightCorner.Y
	);
	core::rect<s32> right(
		AbsoluteRect.LowerRightCorner.X-180,
		AbsoluteRect.UpperLeftCorner.Y,
		AbsoluteRect.LowerRightCorner.X,
		AbsoluteRect.LowerRightCorner.Y
	);

	driver->draw2DRectangle(left, GUI_BG_TOP, GUI_BG_TOP, GUI_BG_BTM, GUI_BG_BTM, &AbsoluteClippingRect);
	driver->draw2DRectangleOutline(left, GUI_BORDER);

	driver->draw2DRectangle(right, GUI_BG_TOP, GUI_BG_TOP, GUI_BG_BTM, GUI_BG_BTM, &AbsoluteClippingRect);
	driver->draw2DRectangleOutline(right, GUI_BORDER);

	video::ITexture *texture = driver->getTexture(getTexturePath("menulogo.png").c_str());
	if (texture != 0) {
		const core::dimension2d<u32>& img_origsize = texture->getOriginalSize();
		core::rect<s32> logo(
			AbsoluteRect.UpperLeftCorner.X,
			AbsoluteRect.UpperLeftCorner.Y,
			AbsoluteRect.UpperLeftCorner.X+200,
			AbsoluteRect.UpperLeftCorner.Y+200
		);
		const video::SColor color(255,255,255,255);
		const video::SColor colors[] = {color,color,color,color};
		driver->draw2DImage(texture, logo, core::rect<s32>(core::position2d<s32>(0,0),img_origsize), NULL, colors, true);
	}

	gui::IGUIElement::draw();

	if (!m_model)
		return;

	m_model_rotation++;
	m_model->setRotation(v3f(0,m_model_rotation,0));
}

void GUICharDefMenu::acceptInput()
{
	//{
		//gui::IGUIElement *e = getElementFromId(GUI_ID_NAME_INPUT);
		//if (e != NULL) {
			//m_data->name = e->getText();
		//}else if (m_data->name == L"") {
			//m_data->name = std::wstring(L"singleplayer");
		//}
	//}


	m_accepted = true;
}

bool GUICharDefMenu::OnEvent(const SEvent& event)
{
	if (event.EventType==EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.Key==KEY_ESCAPE && event.KeyInput.PressedDown) {
			acceptInput();
			quitMenu();
			return true;
		}
	}
	if (event.EventType==EET_GUI_EVENT) {
		if (event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST && isVisible()) {
			if (!canTakeFocus(event.GUIEvent.Element)) {
				dstream<<"GUIMainMenu: Not allowing focus change."<<std::endl;
				// Returning true disables focus change
				return true;
			}
		}
		if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
			switch (event.GUIEvent.Caller->getID()) {
			case GUI_ID_CD_MALE_BUTTON:
			{
				m_parts["gender"] = "M";
				m_parts["Yscale"] = "10";
				m_parts["XZscale"] = "10";
				m_y_scale = 1.0;
				m_xz_scale = 1.0;
				m_model->setMaterialTexture(0, getPlayerTexture());
				m_model->setScale(v3f(m_xz_scale,m_y_scale,m_xz_scale));
				printCharDef();
				return true;
			}
			case GUI_ID_CD_FEMALE_BUTTON:
			{
				m_parts["gender"] = "F";
				m_parts["Yscale"] = "10";
				m_parts["XZscale"] = "9";
				m_y_scale = 1.0;
				m_xz_scale = 0.9;
				m_model->setMaterialTexture(0, getPlayerTexture());
				m_model->setScale(v3f(m_xz_scale,m_y_scale,m_xz_scale));
				printCharDef();
				return true;
			}
			case GUI_ID_CD_TALLER_BUTTON:
			{
				if (m_y_scale >= 1.1)
					return true;
				m_y_scale += 0.1;
				m_parts["Yscale"] = ftos(m_y_scale*10.);
				m_model->setScale(v3f(m_xz_scale,m_y_scale,m_xz_scale));
				printCharDef();
				return true;
			}
			case GUI_ID_CD_SHORTER_BUTTON:
			{
				if (m_y_scale <= 0.8)
					return true;
				m_y_scale -= 0.1;
				m_parts["Yscale"] = ftos(m_y_scale*10.);
				m_model->setScale(v3f(m_xz_scale,m_y_scale,m_xz_scale));
				printCharDef();
				return true;
			}
			case GUI_ID_CD_WIDER_BUTTON:
			{
				if (m_xz_scale >= 1.1)
					return true;
				m_xz_scale += 0.1;
				m_parts["XZscale"] = ftos(m_xz_scale*10.);
				m_model->setScale(v3f(m_xz_scale,m_y_scale,m_xz_scale));
				printCharDef();
				return true;
			}
			case GUI_ID_CD_THINNER_BUTTON:
			{
				if (m_xz_scale <= 0.8)
					return true;
				m_xz_scale -= 0.1;
				m_parts["XZscale"] = ftos(m_xz_scale*10.);
				m_model->setScale(v3f(m_xz_scale,m_y_scale,m_xz_scale));
				printCharDef();
				return true;
			}
			case GUI_ID_CD_SKINTONE_PREV_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_skintone_types.find(m_parts["skintone"]);
				if (i == m_skintone_types.begin())
					i = m_skintone_types.end();
				--i;
				m_parts["skintone"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_SKINTONE_NEXT_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_skintone_types.find(m_parts["skintone"]);
				++i;
				if (i == m_skintone_types.end())
					i = m_skintone_types.begin();
				m_parts["skintone"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_FACE_PREV_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_face_types.find(m_parts["face"]);
				if (i == m_face_types.begin())
					i = m_face_types.end();
				--i;
				m_parts["face"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_FACE_NEXT_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_face_types.find(m_parts["face"]);
				++i;
				if (i == m_face_types.end())
					i = m_face_types.begin();
				m_parts["face"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_HAIRTONE_PREV_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_hairtone_types.find(m_parts["hairtone"]);
				if (i == m_hairtone_types.begin())
					i = m_hairtone_types.end();
				--i;
				m_parts["hairtone"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_HAIRTONE_NEXT_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_hairtone_types.find(m_parts["hairtone"]);
				++i;
				if (i == m_hairtone_types.end())
					i = m_hairtone_types.begin();
				m_parts["hairtone"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_HAIR_PREV_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_hair_types.find(m_parts["hair"]);
				if (i == m_hair_types.begin())
					i = m_hair_types.end();
				--i;
				m_parts["hair"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_HAIR_NEXT_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_hair_types.find(m_parts["hair"]);
				++i;
				if (i == m_hair_types.end())
					i = m_hair_types.begin();
				m_parts["hair"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_EYES_PREV_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_eyes_types.find(m_parts["eyes"]);
				if (i == m_eyes_types.begin())
					i = m_eyes_types.end();
				--i;
				m_parts["eyes"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_EYES_NEXT_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_eyes_types.find(m_parts["eyes"]);
				++i;
				if (i == m_eyes_types.end())
					i = m_eyes_types.begin();
				m_parts["eyes"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_SHIRT_PREV_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_shirt_types.find(m_parts["shirt"]);
				if (i == m_shirt_types.begin())
					i = m_shirt_types.end();
				--i;
				m_parts["shirt"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_SHIRT_NEXT_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_shirt_types.find(m_parts["shirt"]);
				++i;
				if (i == m_shirt_types.end())
					i = m_shirt_types.begin();
				m_parts["shirt"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_PANTS_PREV_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_pants_types.find(m_parts["pants"]);
				if (i == m_pants_types.begin())
					i = m_pants_types.end();
				--i;
				m_parts["pants"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_PANTS_NEXT_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_pants_types.find(m_parts["pants"]);
				++i;
				if (i == m_pants_types.end())
					i = m_pants_types.begin();
				m_parts["pants"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_SHOES_PREV_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_shoes_types.find(m_parts["shoes"]);
				if (i == m_shoes_types.begin())
					i = m_shoes_types.end();
				--i;
				m_parts["shoes"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_SHOES_NEXT_BUTTON:
			{
				std::map<std::string,std::wstring>::iterator i = m_shoes_types.find(m_parts["shoes"]);
				++i;
				if (i == m_shoes_types.end())
					i = m_shoes_types.begin();
				m_parts["shoes"] = i->first;
				m_model->setMaterialTexture(0, getPlayerTexture());
				regenerateGui(m_screensize);
				printCharDef();
				return true;
			}
			case GUI_ID_CD_SAVE_BUTTON:
			{
				savePlayerSkin();
				acceptInput();
				quitMenu();
				return true;
			}
			case GUI_ID_CD_QUIT_BUTTON:
			{
				acceptInput();
				quitMenu();
				return true;
			}
			}
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}

std::string GUICharDefMenu::getPlayerSkin()
{
	std::string tex = "";

	tex += std::string("skins")+DIR_DELIM+"skintone_"+m_parts["skintone"]+".png";
	tex += std::string("^skins")+DIR_DELIM+"gender_"+m_parts["gender"]+".png";
	tex += std::string("^skins")+DIR_DELIM+"face_"+m_parts["face"]+"_"+m_parts["skintone"]+"_"+m_parts["gender"]+".png";
	tex += std::string("^skins")+DIR_DELIM+"eyes_"+m_parts["eyes"]+".png";
	tex += std::string("^clothes_player_pants_canvas_")+m_parts["pants"]+".png";
	tex += std::string("^clothes_player_tshirt_cotton_")+m_parts["shirt"]+".png";
	tex += std::string("^skins")+DIR_DELIM+"hair_"+m_parts["hair"]+"_"+m_parts["hairtone"]+"_"+m_parts["gender"]+".png";
	tex += std::string("^clothes_player_shoes_")+m_parts["shoes"]+".png";

	return tex;
}

//gender:Yscale:XZscale:skintone:eyes:hairtone:hair:face:shirt-colour:pants-colour:shoe-type
//M:10:10:fair:blue:brown:medium:normal:green:blue:leather
void GUICharDefMenu::fetchPlayerSkin()
{
	std::string chardef = std::string(PLAYER_DEFAULT_CHARDEF);
	if (g_settings->exists("character_definition"))
		chardef = g_settings->get("character_definition");
	Strfnd f(chardef);

	m_parts["gender"] = f.next(":");
	m_parts["Yscale"] = f.next(":"); // y scale
	m_parts["XZscale"] = f.next(":"); // xz scale
	m_parts["skintone"] = f.next(":");
	m_parts["eyes"] = f.next(":");
	m_parts["hairtone"] = f.next(":");
	m_parts["hair"] = f.next(":");
	m_parts["face"] = f.next(":");
	m_parts["shirt"] = f.next(":");
	m_parts["pants"] = f.next(":");
	m_parts["shoes"] = f.next(":");

	if (m_parts["gender"] != "M" && m_parts["gender"] != "F")
		m_parts["gender"] = "M";

	m_y_scale = mystof(m_parts["Yscale"])/10.;
	if (m_y_scale < 0.8)
		m_y_scale = 0.8;
	if (m_y_scale > 1.1)
		m_y_scale = 1.1;
	m_parts["Yscale"] = ftos(m_y_scale*10.);

	m_xz_scale = mystof(m_parts["XZscale"])/10.;
	if (m_xz_scale < 0.8)
		m_xz_scale = 0.8;
	if (m_xz_scale > 1.1)
		m_xz_scale = 1.1;
	m_parts["XZscale"] = ftos(m_xz_scale*10.);

	if (getPath("skin",std::string("skintone_")+m_parts["skintone"]+"_"+m_parts["gender"]+".png",true) == "")
		m_parts["skintone"] = "fair";
	if (getPath("skin",std::string("eyes_")+m_parts["eyes"]+".png",true) == "")
		m_parts["eyes"] = "blue";
	if (
		m_parts["hairtone"] == ""
		|| m_parts["hair"] == ""
		|| getPath("skin",std::string("hair_")+m_parts["hair"]+"_"+m_parts["hairtone"]+"_"+m_parts["gender"]+".png",true) == ""
	) {
		m_parts["hairtone"] = "brown";
		m_parts["hair"] = "medium";
	}
	if (getPath("skin",std::string("face_")+m_parts["face"]+"_"+m_parts["gender"]+".png",true) == "")
		m_parts["face"] = "human";

	if (getPath("texture",std::string("clothes_player_tshirt_cotton_")+m_parts["shirt"]+".png",true) == "")
		m_parts["shirt"] = "green";
	if (getPath("texture",std::string("clothes_player_pants_canvas_")+m_parts["pants"]+".png",true) == "")
		m_parts["pants"] = "blue";
	if (getPath("texture",std::string("clothes_player_shoes_")+m_parts["shoes"]+".png",true) == "")
		m_parts["shoes"] = "leather";
}

//gender:Yscale:XZscale:skintone:eyes:hairtone:hair:face
//M:10:10:fair:blue:brown:medium:normal
void GUICharDefMenu::savePlayerSkin()
{
	std::string chardef = m_parts["gender"];
	chardef += std::string(":")+m_parts["Yscale"];
	chardef += std::string(":")+m_parts["XZscale"];
	chardef += std::string(":")+m_parts["skintone"];
	chardef += std::string(":")+m_parts["eyes"];
	chardef += std::string(":")+m_parts["hairtone"];
	chardef += std::string(":")+m_parts["hair"];
	chardef += std::string(":")+m_parts["face"];
	chardef += std::string(":")+m_parts["shirt"];
	chardef += std::string(":")+m_parts["pants"];
	chardef += std::string(":")+m_parts["shoes"];

	g_settings->set("character_definition",chardef);
}
