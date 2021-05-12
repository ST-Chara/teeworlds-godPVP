#include "weapon.h"

CWeapon::CWeapon(CCharacter *pOwnerChar)
{
	m_pOwnerChar = pOwnerChar;
	m_pGameServer = pOwnerChar->GameServer();
	m_pGameWorld = pOwnerChar->GameWorld();
	m_pServer = pOwnerChar->Server();
	m_Ammo = -1;
	m_MaxAmmo = 10;
	m_FullAuto = false;
	m_AmmoRegenTime = 0;
	m_AmmoRegenStart = 0;
	m_ReloadTimer = 0;
	m_AttackTick = 0;
	m_LastNoAmmoSound = 0;
	m_FireDelay = 0;
	m_WeaponAquiredTick = m_pServer->Tick();
}

void CWeapon::Tick()
{
	if(m_ReloadTimer > 0)
	{
		m_ReloadTimer--;
		return;
	}

	if(m_AmmoRegenTime && m_Ammo >= 0)
	{
		if(m_ReloadTimer <= 0)
		{
			if(m_AmmoRegenStart < 0)
				m_AmmoRegenStart = Server()->Tick();

			if((Server()->Tick() - m_AmmoRegenStart) >= m_AmmoRegenTime * Server()->TickSpeed() / 1000)
			{
				// Add some ammo
				m_Ammo = minimum(m_Ammo + 1, m_MaxAmmo);
				m_AmmoRegenStart = -1;
			}
		}
		else
		{
			m_AmmoRegenStart = -1;
		}
	}
}

void CWeapon::TickPaused()
{
	++m_AttackTick;
	if(m_AmmoRegenStart > -1)
		++m_AmmoRegenStart;
	++m_WeaponAquiredTick;
}

void CWeapon::HandleFire(vec2 Direction)
{
	if(m_ReloadTimer > 0)
		return;

	if(m_Ammo == 0)
	{
		m_ReloadTimer = 125 * Server()->TickSpeed() / 1000;
		if(m_LastNoAmmoSound + Server()->TickSpeed() <= Server()->Tick())
		{
			GameWorld()->CreateSound(Pos(), SOUND_WEAPON_NOAMMO);
			m_LastNoAmmoSound = Server()->Tick();
		}
		return;
	}

	Fire(Direction);

	m_AttackTick = Server()->Tick();
	if(m_Ammo > 0)
		m_Ammo -= 1;

	if(m_ReloadTimer == 0)
		m_ReloadTimer = m_FireDelay * Server()->TickSpeed() / 1000;
}

vec2 CWeapon::Pos() { return Character()->m_Pos; }
float CWeapon::GetProximityRadius() { return Character()->GetProximityRadius(); }
