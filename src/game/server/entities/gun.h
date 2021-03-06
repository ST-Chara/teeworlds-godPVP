/* copyright (c) 2007 magnus auvinen, see licence.txt for more info */

#ifndef GAME_SERVER_ENTITIES_GUN_H
#define GAME_SERVER_ENTITIES_GUN_H

#include <game/gamecore.h>
#include <game/server/entity.h>

class CCharacter;

class CGun : public CEntity
{
	int m_EvalTick;

	vec2 m_Core;
	bool m_Freeze;
	bool m_Explosive;

	void Fire();
	int m_LastFire;

	int m_ID;

public:
	CGun(CGameWorld *pGameWorld, vec2 Pos, bool Freeze, bool Explosive, int Layer = 0, int Number = 0);
	~CGun();

	virtual void Reset() override;
	virtual void Tick() override;
	virtual void Snap(int SnappingClient, int OtherMode) override;
};

#endif // GAME_SERVER_ENTITIES_GUN_H
