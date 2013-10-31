#ifndef CIRCUIT_HEADER
#define CIRCUIT_HEADER

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
	virtual bool step(float dtime);

	virtual bool energise(u8 level, v3s16 powersrc, v3s16 signalsrc, v3s16 pos)
	{
		if (!level || m_energy < level)
			m_energy = level;
		return true;
	}
	virtual u8 getEnergy()
	{
		return m_energy;
	}

private:
	u8 m_energy;
};

namespace circuit
{

bool propogate_energy(u8 level, v3s16 powersrc, v3s16 pos);

}

#endif
