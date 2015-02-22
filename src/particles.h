/************************************************************************
* Minetest-c55
* Copyright (C) 2012 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* particles.h
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

#ifndef PARTICLES_HEADER
#define PARTICLES_HEADER

#define DIGGING_PARTICLES_AMOUNT 64

#include <iostream>
#include "common_irrlicht.h"
#include "tile.h"
#include "player.h"
#include "environment.h"

class Particle : public scene::ISceneNode
{
	public:
	Particle(
		scene::ISceneManager* mgr,
		LocalPlayer *player,
		s32 id,
		v3f pos,
		v3f velocity,
		v3f acceleration,
		float expirationtime,
		float size,
		AtlasPointer *texture
	);
	~Particle();

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return m_box;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return m_material;
	}

	virtual void OnRegisterSceneNode();
	virtual void render();

	void updateCameraOffset(v3s16 camera_offset)
	{
		m_camera_offset = camera_offset;
	}

	void step(float dtime, ClientEnvironment &env);

	bool get_expired ()
	{ return m_expiration < m_time; }

private:
	video::S3DVertex m_vertices[4];
	float m_time;
	float m_expiration;

	core::aabbox3d<f32> m_box;
	core::aabbox3d<f32> m_collisionbox;
	video::SMaterial m_material;
	v3f m_pos;
	v3s16 m_camera_offset;
	v3f m_velocity;
	v3f m_acceleration;
	LocalPlayer *m_player;
	float m_size;
	AtlasPointer *m_ap;
	u8 m_light;
};

class ParticleSpawner
{
public:
	ParticleSpawner(
		scene::ISceneManager *smgr,
		LocalPlayer *player,
		u16 amount,
		float time,
		v3f minp, v3f maxp,
		v3f minvel, v3f maxvel,
		v3f minacc, v3f maxacc,
		float minexptime, float maxexptime,
		float minsize, float maxsize,
		bool collisiondetection,
		AtlasPointer *texture,
		u32 id
	);

	~ParticleSpawner();

	void step(float dtime, ClientEnvironment &env);

	bool get_expired () { return (m_amount <= 0) && m_spawntime != 0; }

private:
	float m_time;
	scene::ISceneManager *m_smgr;
	LocalPlayer *m_player;
	u16 m_amount;
	float m_spawntime;
	v3f m_minpos;
	v3f m_maxpos;
	v3f m_minvel;
	v3f m_maxvel;
	v3f m_minacc;
	v3f m_maxacc;
	float m_minexptime;
	float m_maxexptime;
	float m_minsize;
	float m_maxsize;
	AtlasPointer *m_ap;
	std::vector<float> m_spawntimes;
	bool m_collisiondetection;
};

void allparticles_step (float dtime, ClientEnvironment &env);
void allparticlespawners_step (float dtime, ClientEnvironment &env);
void delete_particlespawner(u32 id);
u32 add_particlespawner(scene::ISceneManager *smgr, LocalPlayer *player,
	u16 amount, float time,
	v3f minpos, v3f maxpos, v3f minvel, v3f maxvel, v3f minacc, v3f maxacc,
	float minexptime, float maxexptime, float minsize, float maxsize,
	AtlasPointer *ap);
void clear_particles();

void addDiggingParticles(scene::ISceneManager* smgr, LocalPlayer *player, v3s16 pos, const TileSpec tiles[]);
void addPunchingParticles(scene::ISceneManager* smgr, LocalPlayer *player, v3s16 pos, const TileSpec tiles[]);
void addNodeParticle(scene::ISceneManager* smgr, LocalPlayer *player, v3s16 pos, const TileSpec tiles[]);
void update_particles_camera_offset(v3s16 camera_offset);

#endif

