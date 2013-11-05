#ifndef CIRCUIT_HEADER
#define CIRCUIT_HEADER

#include <map>
#include "mapnode.h"
#include "content_mapnode.h"
#include "nodemetadata.h"

class CircuitNodeMetadata : public NodeMetadata
{
public:
	CircuitNodeMetadata();
	~CircuitNodeMetadata();

	virtual u16 typeId() const;
	static NodeMetadata* create(std::istream &is);
	virtual NodeMetadata* clone();
	virtual void serializeBody(std::ostream &os);
	virtual bool step(float dtime) {return false;};

	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
	virtual u8 getEnergy()
	{
		return m_energy;
	}

private:
	u8 m_energy;
	std::map<v3s16, u8> m_sources;
};

class Environment;

class CircuitManager
{
public:
	CircuitManager(Environment *env);
	~CircuitManager();

	bool energise(u8 level, v3s16 powersrc, v3s16 pos);
	bool energise(u8 level, v3f powersrc, v3s16 pos)
	{
		v3s16 p;
		p.X = powersrc.X;
		p.Y = powersrc.Y;
		p.Z = powersrc.Z;
		return energise(level,p,pos);
	}
	bool connect(v3s16 pos);
	bool disconnect(v3s16 pos);

private:
	bool propogate(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos);
	Environment *m_env;
	std::map<v3s16, bool> m_ignore_nodes;
};

#endif
