//Globals

#include "Includes.h"
#define RandomInt(min, max) (rand() % (max - min + 1) + min)

bool aim_enable = false;
ScreenSize_t ScreenSize2;

namespace Math
{
	FORCEINLINE float FastSqrt(float x)
	{
		__m128 root = _mm_sqrt_ss(_mm_load_ss(&x));
		return *(reinterpret_cast<float *>(&root));
	}

	FORCEINLINE float FastRSqrtFast(float x)
	{
		// use intrinsics
		__m128 rroot = _mm_rsqrt_ss(_mm_load_ss(&x));
		return *(reinterpret_cast<float *>(&rroot));
	}

	void SinCos(float radians, float *sine, float *cosine)
	{
		_asm
		{
			fld		DWORD PTR[radians]
				fsincos

				mov edx, DWORD PTR[cosine]
				mov eax, DWORD PTR[sine]

				fstp DWORD PTR[edx]
				fstp DWORD PTR[eax]
		}
	}

	FORCEINLINE float DotProduct(const Vector& a, const Vector& b)
	{
		return(a.x*b.x + a.y*b.y + a.z*b.z);
	}

	FORCEINLINE void CrossProduct(const Vector& a, const Vector& b, Vector& result)
	{
		result.x = a.y*b.z - a.z*b.y;
		result.y = a.z*b.x - a.x*b.z;
		result.z = a.x*b.y - a.y*b.x;
	}

	FORCEINLINE void VectorDivide(const Vector& a, float b, Vector& c)
	{
		float oob = 1.0f / b;
		c.x = a.x * oob;
		c.y = a.y * oob;
		c.z = a.z * oob;
	}

	FORCEINLINE void VectorDivide(const Vector& a, const Vector& b, Vector& c)
	{
		c.x = a.x / b.x;
		c.y = a.y / b.y;
		c.z = a.z / b.z;
	}

	FORCEINLINE void VectorAdd(const Vector& a, const Vector& b, Vector& c)
	{
		c.x = a.x + b.x;
		c.y = a.y + b.y;
		c.z = a.z + b.z;
	}

	FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
	{
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}

	FORCEINLINE void VectorMultiply(const Vector& a, float b, Vector& c)
	{
		c.x = a.x * b;
		c.y = a.y * b;
		c.z = a.z * b;
	}

	FORCEINLINE void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
	{
		c.x = a.x * b.x;
		c.y = a.y * b.y;
		c.z = a.z * b.z;
	}

	FORCEINLINE float VectorLength(const Vector& v)
	{
		return (float)FastSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	}

	float VectorNormalize(Vector& vec)
	{
		float radius = sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);

		// FLT_EPSILON is added to the radius to eliminate the possibility of divide by zero.
		float iradius = 1.f / (radius + FLT_EPSILON);

		vec.x *= iradius;
		vec.y *= iradius;
		vec.z *= iradius;

		return radius;
	}

	void VectorTransform(Vector in, matrix3x4_t &matrix, Vector &out)
	{
		Vector a = Vector(matrix[0][0], matrix[0][1], matrix[0][2]);
		Vector b = Vector(matrix[1][0], matrix[1][1], matrix[1][2]);
		Vector c = Vector(matrix[2][0], matrix[2][1], matrix[2][2]);

		out = Vector(in.Dot(a) + matrix[0][3], in.Dot(b) + matrix[1][3], in.Dot(c) + matrix[2][3]);
	}

	void VectorAngles(Vector forward, Vector &angles)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	void AngleVectors(Vector& angles, Vector *forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles.y), &sy, &cy);
		SinCos(DEG2RAD(angles.x), &sp, &cp);

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	void MatrixAngles(const matrix3x4_t& matrix, Vector &angles)
	{
		float forward[3];
		float left[3];
		float up[3];

		forward[0] = matrix[0][0];
		forward[1] = matrix[1][0];
		forward[2] = matrix[2][0];
		left[0] = matrix[0][1];
		left[1] = matrix[1][1];
		left[2] = matrix[2][1];
		up[2] = matrix[2][2];

		float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

		if (xyDist > 0.001f)
		{
			angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));
			angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));
			angles[2] = RAD2DEG(atan2f(left[2], up[2]));
		}
		else
		{
			angles[1] = RAD2DEG(atan2f(-left[0], left[1]));
			angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));
			angles[2] = 0;
		}
	}

	void MatrixPosition(const matrix3x4_t &matrix, Vector &position)
	{
		position.x = matrix[0][3];
		position.y = matrix[1][3];
		position.z = matrix[2][3];
	}

	void MatrixAngles(const matrix3x4_t &matrix, Vector &angles, Vector &position)
	{
		MatrixAngles(matrix, angles);
		MatrixPosition(matrix, position);
	}

	void VectorScale(Vector& input, float length)
	{
		input.x = input.x / length;
		input.y = input.y / length;
		input.z = input.z / length;
	}

	void NormalizeAngles(Vector& angles)
	{
		while (angles.x > 180.0f)
			angles.x -= 360.0f;

		while (angles.x < -180.0f)
			angles.x += 360.0f;

		while (angles.y > 180.0f)
			angles.y -= 360.0f;

		while (angles.y < -180.0f)
			angles.y += 360.0f;

		while (angles.z > 180.0f)
			angles.z -= 360.0f;

		while (angles.z < -180.0f)
			angles.z += 360.0f;
	}

	void ClampAngles(Vector& angles)
	{
		if (angles.x > 89.0f)
			angles.x = 89.0f;
		else if (angles.x < -89.0f)
			angles.x = -89.0f;

		while (angles.y > 180.0f)
			angles.y -= 360.0f;

		while (angles.y < -180.0f)
			angles.y += 360.0f;

		angles.z = 0;
	}
}

/* Helper Functions */
wchar_t* Wchar_ret(const char* in)
{
	wchar_t* out;
	int strsize = MultiByteToWideChar(CP_UTF8, 0, in, strlen(in) + 1, NULL, 0);
	out = new wchar_t[strsize];
	MultiByteToWideChar(CP_UTF8, 0, in, strlen(in) + 1, out, strsize);
	return out;
}

void DrawLine(int x, int y, int x2, int y2, int r, int g, int b, int a){
	Interface.Surface->DrawSetColor(r, g, b, a);
	Interface.Surface->DrawLine(x, y, x2, y2);
}

void TextW(bool cent, unsigned long font, int x, int y, int r, int g, int b, int a, wchar_t* pszString)
{
	if (cent)
	{
		int wide, tall;
		Interface.Surface->GetTextSize(font, pszString, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}

	Interface.Surface->DrawSetTextColor(r, g, b, a);
	Interface.Surface->DrawSetTextFont(font);
	Interface.Surface->DrawSetTextPos(x, y);
	Interface.Surface->DrawPrintText(pszString, (int)wcslen(pszString));
}

void Text(int x, int y, int r, int g, int b, int a, bool bCenter, unsigned long font, const char *fmt, ...)
{
	if (!font)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };

	va_start(va_alist, fmt);
	vsprintf(szBuffer, fmt, va_alist);
	va_end(va_alist);

	wchar_t* pszStringWide = reinterpret_cast<wchar_t*>(malloc((strlen(szBuffer) + 1) * sizeof(wchar_t)));
	mbstowcs(pszStringWide, szBuffer, (strlen(szBuffer) + 1) * sizeof(wchar_t));
	TextW(bCenter, font, x, y, r, g, b, a, pszStringWide);
	delete[] pszStringWide;
}

void Text(int x, int y, int r, int g, int b, int a, bool bCenter, unsigned long font, const wchar_t *fmt, ...)
{
	if (!font)
		return;

	va_list va_alist;
	wchar_t szBuffer[1024] = { '\0' };

	va_start(va_alist, fmt);
	wvsprintfW(szBuffer, fmt, va_alist);
	va_end(va_alist);

	TextW(bCenter, font, x, y, r, g, b, a, szBuffer);
}

void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{
	Interface.Surface->DrawSetColor(r, g, b, a);
	Interface.Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Border(int x, int y, int w, int h, int line, int r, int g, int b, int a)
{
	FillRGBA(x, y, w, line, r, g, b, a);
	FillRGBA(x, y, line, h, r, g, b, a);
	FillRGBA(x + w, y, line, h, r, g, b, a);
	FillRGBA(x, y + h, w + line, line, r, g, b, a);
}


/* Define Local Player */
CEntity* Me(void)
{
	return Interface.EntityList->GetClientEntity(Interface.EngineClient->GetLocalPlayer());
}

bool WorldToScreen(const Vector &point, Vector &screen)
{
	bool behind;
	float w;
	const VMatrix &worldToScreen = Interface.EngineClient->WorldToScreenMatrix();
	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	if (w < 0.001f)
	{
		behind = false;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = true;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;

		int iScreenWidth, iScreenHeight;

		Interface.EngineClient->GetScreenSize(iScreenWidth, iScreenHeight);
		float x = iScreenWidth / 2;
		float y = iScreenHeight / 2;
		x += 0.5 * screen.x * iScreenWidth + 0.5;
		y -= 0.5 * screen.y * iScreenHeight + 0.5;
		screen.x = x;
		screen.y = y;
	}

	return behind;
}

void ValidateTarget(Vector& vAbsStart, Vector& vAbsEnd, CUserCmd* cmd)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	player_info_t playerInfo;
	CEntity* Entity;
	filter.pSkip = Me();

	ray.Init(vAbsStart, vAbsEnd);
	Interface.EngineTrace->TraceRay(ray, 0x4600400B, &filter, &tr);

	Entity = tr.m_pEnt;

	if ((Entity != NULL//invalid
		&& !Entity->IsDormant()//dormant
		&& Entity->GetLifeState()))//not alive
	{
		if ((Entity->GetTeamID() != Me()->GetTeamID()//not a enemy
			&& (Entity->GetRenderColor().a() == 255)//spawn protected
			&& (Entity->GetTeamID() >= 2 && Entity->GetTeamID() <= 3)))//not t or ct
		{
			if (Me()->GetLifeState())
			{
				if (Interface.Settings->head)
				{
					if (tr.hitbox == 1 || tr.hitbox == 0)
						cmd->buttons |= IN_ATTACK;
				}
				if (Interface.Settings->neck)
				{
					if (tr.hitbox == 2)
						cmd->buttons |= IN_ATTACK;
				}
				if (Interface.Settings->body)
				{
					if (tr.hitbox >= 3 && tr.hitbox <= 7)
						cmd->buttons |= IN_ATTACK;
				}
				if (Interface.Settings->arms)
				{
					if (tr.hitbox >= 14 && tr.hitbox <= 19)
						cmd->buttons |= IN_ATTACK;
				}
				if (Interface.Settings->legs)
				{
					if (tr.hitbox >= 8 && tr.hitbox <= 13)
						cmd->buttons |= IN_ATTACK;
				}
			}

		}
	}


}

bool GetVisible(Vector& vAbsStart, Vector& vAbsEnd, CEntity* pEntity)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = Me();

	ray.Init(vAbsStart, vAbsEnd);
	Interface.EngineTrace->TraceRay(ray, 0x4600400B, &filter, &tr);

	return (tr.m_pEnt == pEntity || tr.fraction > 0.99f);
}

bool GetMultiPointHitboxPosition(CEntity* ClientEntity, int iHitBox, Vector* vOut)//corner stuff stolen from kiro
{
	matrix3x4_t pmatrix[128];

	Vector vMin, vMax;

	const void* model = ClientEntity->GetModel();

	if (!model)
		return false;

	studiohdr_t *pStudioHdr = Interface.ModelInfo->GetStudiomodel(model);

	if (pStudioHdr == NULL)
		return false;

	if (!ClientEntity->SetupBones(pmatrix, 128, 0x100, 0))
		return false;

	mstudiohitboxset_t *set = pStudioHdr->pHitboxSet(ClientEntity->GetHitboxSet());

	if (!set)
		return false;

	mstudiobbox_t* pbox = set->pHitbox(iHitBox);

	if (!pbox)
		return false;

	// center and all the points of the hitbox
	Vector points[9] = { ((pbox->bbmin + pbox->bbmax) * .5f), // center
		Vector(pbox->bbmin.x, pbox->bbmin.y, pbox->bbmin.z), // left bottom back corner
		Vector(pbox->bbmin.x, pbox->bbmax.y, pbox->bbmin.z), // left bottom front corner
		Vector(pbox->bbmax.x, pbox->bbmax.y, pbox->bbmin.z), // left top front corner
		Vector(pbox->bbmax.x, pbox->bbmin.y, pbox->bbmin.z), // left top back corner
		Vector(pbox->bbmax.x, pbox->bbmax.y, pbox->bbmax.z), // right top front corner
		Vector(pbox->bbmin.x, pbox->bbmax.y, pbox->bbmax.z), // right bottom front corner
		Vector(pbox->bbmin.x, pbox->bbmin.y, pbox->bbmax.z), // right bottom back corner
		Vector(pbox->bbmax.x, pbox->bbmin.y, pbox->bbmax.z)  // right top back corner
	};

	for (int index = 0; index <= 8; ++index)
	{
		if (index != 0)
		{
			// scale down the hitbox size
			points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);
		}

		// transform the vector
		Math::VectorTransform(points[index], pmatrix[pbox->bone], vOut[index]);
	}

	return true;
}

void RunPred(CUserCmd* cmd)
{
	BYTE bMoveData[0x200];
	float curtime = Interface.GlobalVars->curtime;
	float frametime = Interface.GlobalVars->frametime;
	int iFlags = Me()->GetFlags();

	Interface.GlobalVars->curtime = Me()->GetTickBase() * Interface.GlobalVars->interval_per_tick;
	Interface.GlobalVars->frametime = Interface.GlobalVars->interval_per_tick;

	Interface.MoveHelper->SetHost(Me());

	Interface.Prediction->SetupMove(Me(), cmd, NULL, &bMoveData);
	Interface.GameMovement->ProcessMovement(Me(), &bMoveData);
	Interface.Prediction->FinishMove(Me(), cmd, &bMoveData);

	Interface.MoveHelper->SetHost(0);

	Interface.GlobalVars->curtime = curtime;
	Interface.GlobalVars->frametime = frametime;
	Me()->OverrideNetVar<int>(Interface.Offsets->Flags, iFlags);
}

float GetFov(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float fov;

	//Utilities::DebugPrint("Source: %f, %f, %f", src.x, src.y, src.z);
	//Utilities::DebugPrint("Dest: %f, %f, %f", dst.x, dst.y, dst.z);

	Vector tmp = dst - src;
	Math::VectorAngles(tmp, ang);
	Vector vTempAngles = ang - angle;
	Math::NormalizeAngles(vTempAngles);

	//Utilities::DebugPrint("FOV: %f", vTempAngles.Length());

	return vTempAngles.Length();
}

void SayInChat(const char *text)
{
	char buffer[256];
	sprintf_s(buffer, "say \"%s\"", text);
	Interface.EngineClient->ClientCmd_Unrestricted(buffer);
}

void SetClanTag(const char* tag, const char* name)
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("engine.dll")) + 0x0088640);

	pSetClanTag(tag, name);
}

void RecoilDot()
{
	INT x = (INT)ScreenSize2.Width * 0.5f;
	INT y = (INT)ScreenSize2.Height * 0.5f;
	INT dx = (INT)ScreenSize2.Width / 90;
	INT dy = (INT)ScreenSize2.Height / 90;
	x -= (INT)dx * Me()->GetPunchAngle()[1];
	y += (INT)dy * Me()->GetPunchAngle()[0];
	if (Me())
	{
		Border(x - 2, y - 2, 2, 2, 3, 0, 165, 255, 255);
		if (aim_enable)
		{
			Border(x - 1, y - 1, 2, 2, 1, 255, 0, 0, 255);
		}
		else
		{
			Border(x - 1, y - 1, 2, 2, 1, 0, 165, 255, 255);
		}

	}
}

void DrawGlow(CEntity* Entity, int r, int g, int b, int a)
{
	//m_flFlashDuration + 0x18
	int iEntity_glow_index = Entity->ReadNetVar<int>(Interface.Offsets->GlowIndex);

	typedef uintptr_t(__stdcall* fnGetGlowObjectManager)();
	fnGetGlowObjectManager pGetObjMngr = (fnGetGlowObjectManager)dwGlowBase;
	GlowObjectDefinition* glow_object_array = *(GlowObjectDefinition**)pGetObjMngr();

	glow_object_array[iEntity_glow_index].m_bRenderWhenOccluded = !Entity->IsDormant();
	glow_object_array[iEntity_glow_index].m_bRenderWhenUnoccluded = false;
	glow_object_array[iEntity_glow_index].m_bFullBloomRender = false;
	glow_object_array[iEntity_glow_index].m_flRed = (r / 255.0f); // glow_object_array[iEntity_glow_index].m_flRed = (r / 255.0f);
	glow_object_array[iEntity_glow_index].m_flGreen = (g / 255.0f); // glow_object_array[iEntity_glow_index].m_flGreen = (g / 255.0f);
	glow_object_array[iEntity_glow_index].m_flBlue = (b / 255.0f); // glow_object_array[iEntity_glow_index].m_flBlue = (b / 255.0f);
	glow_object_array[iEntity_glow_index].m_flAlpha = (a / 255.0f); // glow_object_array[iEntity_glow_index].m_flAlpha = (a / 255.0f);
}

//Antiaim
void BackJitter(CUserCmd *cmd)
{
	if (!Interface.Settings->antiaim)
		return;
	if (cmd->buttons & IN_ATTACK)
		return;
	int random = rand() % 100;

	// Small chance of starting fowards
	if (random < 98)
		// Look backwards
		cmd->viewangles.y -= 180;

	// Some gitter
	if (random < 15)
	{
		float change = -70 + (rand() % (int)(140 + 1));
		cmd->viewangles.y += change;
	}
	if (random == 69)
	{
		float change = -90 + (rand() % (int)(180 + 1));
		cmd->viewangles.y += change;
	}


}

void Flip(CUserCmd *cmd)
{
	if (!Interface.Settings->antiaim)
		return;
	if (cmd->buttons & IN_ATTACK)
		return;
	static bool back = false;
	back = !back;
	if (back)
		cmd->viewangles.y -= 180;
}

void FastSpin(CUserCmd *cmd)
{
	if (!Interface.Settings->antiaim)
		return;
	if (cmd->buttons & IN_ATTACK)
		return;
	int random = 140 + rand() % 40;
	static float current_y = cmd->viewangles.y;
	current_y += random;
	cmd->viewangles.y = current_y;
}

void PitchUp(CUserCmd *cmd)
{
	if (!Interface.Settings->antiaim)
		return;
	if (cmd->buttons & IN_ATTACK)
		return;
	cmd->viewangles.x = -89.0f;
}

//Chatspam Strings 
std::string random[5]{
	"Test",
	"Test",
	"Test",
	"Test",
	"Test"
};

//Memory functions
void Aimbot(CUserCmd* cmd)
{
	int iTarget = -1;
	float flDistance = 8192.f;
	player_info_t playerInfo;
	Vector vecTarget;
	Vector vecPreAim;
	long milliseconds = 0;
	long rcstime = 0;
	vecPreAim.Init();

	for (int i = Interface.EntityList->GetHighestEntityIndex(); i > 0; --i)
	{
		if (i == Interface.EngineClient->GetLocalPlayer())
			continue;

		CEntity* Entity = Interface.EntityList->GetClientEntity(i);

		if (Entity == NULL//invalid
			|| Entity->IsDormant()//dormant
			|| !Interface.EngineClient->GetPlayerInfo(i, &playerInfo)//not a player
			|| !Entity->GetLifeState())//not alive
			continue;

		if (Entity->GetTeamID() == Me()->GetTeamID()//not a enemy
			|| (Entity->GetRenderColor().a() < 255)//spawn protected
			|| (Entity->GetTeamID() < 2 && Entity->GetTeamID() > 3))//not t or ct
			continue;

		Vector out[9];

		if (!GetMultiPointHitboxPosition(Entity, Interface.Settings->bone, out))
			continue;

		if (!GetVisible(Me()->GetEyePos(), out[0], Entity))
			continue;

		Vector temp = cmd->viewangles;
		temp.x += Me()->GetPunchAngle().x * 2.0f;
		temp.y += Me()->GetPunchAngle().y * (0.02f * Interface.Settings->intens_x);

		float flFieldofView = GetFov(temp, Me()->GetEyePos(), out[0]);


		if (flFieldofView <= Interface.Settings->fov && flFieldofView < flDistance)
		{
			flDistance = flFieldofView;
			iTarget = i;
			vecTarget = out[0];
		}
	}

	if (iTarget > -1 && (Interface.Settings->aim) && (Me()->HasAmmo()))
	{
		aim_enable = true;
		Vector tmp = vecTarget - Me()->GetEyePos(), smooth;
		Math::VectorAngles(tmp, vecPreAim);
		smooth = vecPreAim;
		if (Me()->GetShotsFired() >= 2)
		{
			smooth.x -= Me()->GetPunchAngle().x * (0.02f * Interface.Settings->intens_y);
			smooth.y -= Me()->GetPunchAngle().y * (0.02f * Interface.Settings->intens_x);
		}

		Math::NormalizeAngles(smooth);
		cmd->viewangles = smooth;
		if (Interface.Settings->silentaim == false)
			Interface.EngineClient->SetViewAngles(smooth);

		if (Interface.Settings->norecoil == true)
		{
			cmd->viewangles -= Me()->GetPunchAngle() * 2;
		}



		if (Interface.Settings->autoduck == true)
			cmd->buttons |= IN_DUCK;

		if (Interface.Settings->autoshoot == true)
			cmd->buttons |= IN_ATTACK;
	}
	else
	{
		aim_enable = false;
		milliseconds = GetTickCount();
		rcstime = GetTickCount();
	}
}

void Triggerbot(CUserCmd* cmd)
{
	if (Interface.Settings->trigger)
	{
		//flDistance = 8192.f;
		Vector Src, Dst, vAngle;
		CGameTrace trace;


		Src = Me()->GetEyePos();
		vAngle = cmd->viewangles;
		Math::AngleVectors(vAngle, &vAngle);
		vAngle *= 8192.f;

		Dst = Src + vAngle;
		ValidateTarget(Src, Dst, cmd);
	}

}

void ESP()
{
	player_info_t pInfo;
	for (int i = Interface.EntityList->GetHighestEntityIndex(); i > 0; --i)
	{
		if (Interface.EngineClient->GetLocalPlayer() == i)
			continue;

#if defined _DEBUG
		Utilities::DebugPrint("not localplayer");
#endif

		CEntity* Entity = Interface.EntityList->GetClientEntity(i);
		if (Entity == NULL
			|| !Entity->GetLifeState()
			|| !Interface.EngineClient->GetPlayerInfo(i, &pInfo))
			continue;


		Color color(0, 0, 0, 0);
		Vector screen[20];

		if (Entity->GetTeamID() == 2)
		{
			color.SetColor(255, 0, 0, 255);
		}
		else if (Entity->GetTeamID() == 3)
		{
			color.SetColor(0, 192, 255, 255);
		}

#if defined _DEBUG
		Utilities::DebugPrint("Setting Colors");
#endif




		if (Entity->IsDormant())
			continue;

		if (Entity->GetTeamID() != Me()->GetTeamID())
		{
			if (GetVisible(Me()->GetEyePos(), Entity->GetEyePos(), Entity))
				color.SetColor(255, 165, 0, 255);
		}

#if defined _DEBUG
		Utilities::DebugPrint("Setting Visible Colors");
#endif
		auto Health = Entity->GetHealth();

		Vector vMin = Entity->GetMins(), vMax = Entity->GetMaxs(), lbf, lbb, ltb, ltf, rtb, rbb, rbf, rtf, vBounds[] = {
			Vector(vMin.x, vMin.y, vMin.z), // left bottom back corner
			Vector(vMin.x, vMax.y, vMin.z), // left bottom front corner
			Vector(vMax.x, vMax.y, vMin.z), // left top front corner
			Vector(vMax.x, vMin.y, vMin.z), // left top back corner
			Vector(vMax.x, vMax.y, vMax.z), // right top front corner
			Vector(vMin.x, vMax.y, vMax.z), // right bottom front corner
			Vector(vMin.x, vMin.y, vMax.z), // right bottom back corner
			Vector(vMax.x, vMin.y, vMax.z)  // right top back corner
		};

#if defined _DEBUG
		Utilities::DebugPrint("BoundingBox 1/2");
#endif

		if (!WorldToScreen(vBounds[0] + Entity->GetOrigin(), lbb) || !WorldToScreen(vBounds[1] + Entity->GetOrigin(), lbf)
			|| !WorldToScreen(vBounds[2] + Entity->GetOrigin(), ltf) || !WorldToScreen(vBounds[3] + Entity->GetOrigin(), ltb)
			|| !WorldToScreen(vBounds[4] + Entity->GetOrigin(), rtf) || !WorldToScreen(vBounds[5] + Entity->GetOrigin(), rbf)
			|| !WorldToScreen(vBounds[6] + Entity->GetOrigin(), rbb) || !WorldToScreen(vBounds[7] + Entity->GetOrigin(), rtb))
			continue;

		Vector vBoundArray[] = { lbf, rtb, lbb, rtf, rbf, rbb, ltb, ltf };
#if defined _DEBUG
		Utilities::DebugPrint("BoundingBox 2/2");
#endif

		float left = ltf.x;
		float top = ltf.y;
		float right = ltf.x;
		float bottom = ltf.y;

#if defined _DEBUG
		Utilities::DebugPrint("1");
#endif

		for (int i = 1; i < 8; i++)
		{
			if (left > vBoundArray[i].x)
				left = vBoundArray[i].x;
			if (top < vBoundArray[i].y)
				top = vBoundArray[i].y;
			if (right < vBoundArray[i].x)
				right = vBoundArray[i].x;
			if (bottom > vBoundArray[i].y)
				bottom = vBoundArray[i].y;
		}
#if defined _DEBUG
		Utilities::DebugPrint("2");
#endif

		Vector head[9];

		if (!GetMultiPointHitboxPosition(Entity, Interface.Settings->bone, head))
			continue;

		Vector vhead;

		auto FontSize = 12; //change this if you change the font

		if (Me()->GetTeamID() != Entity->GetTeamID())
		{
			//Utilities::DebugPrint("Drawing");

			if (Interface.Settings->box && Interface.Settings->esp)
			{
				Border(left - 3, bottom - 3, right - left, top - bottom, 3, 0, 0, 0, 255);
				Border(left - 2, bottom - 2, right - left, top - bottom, 1, color.r(), color.g(), color.b(), color.a());
			}
			if (Interface.Settings->name && Interface.Settings->esp)
				Text(left + ((right - left) / 2), bottom - 8, 255, 255, 255, 255, true, Font, Wchar_ret(pInfo.name));

			if (Interface.Settings->healthbar && Interface.Settings->esp)
			{
				int r = 255 - (Health * 2.55);
				int g = Health * 2.55;
				FillRGBA(left - 10, (bottom - (FontSize - 2)) + 6, 5, (top - bottom) + 3, 0, 0, 0, 255);
				FillRGBA(left - 9, (bottom - (FontSize - 2)) + 7, 3, (top - bottom) + 1, r, g, 0, 255);
				FillRGBA(left - 9, (bottom - (FontSize - 2)) + 7, 3, (top - bottom) - ((top - bottom) * ((float)Health / 100.0f)), 0, 0, 0, 255);
			}

			DrawLine(screen[0].x, screen[0].y, screen[7].x, screen[7].y, 255, 255, 255, 255);

			DrawLine(screen[6].x, screen[6].y, screen[7].x, screen[7].y, 255, 255, 255, 255);
			DrawLine(screen[6].x, screen[6].y, screen[5].x, screen[5].y, 255, 255, 255, 255);
			DrawLine(screen[4].x, screen[4].y, screen[5].x, screen[5].y, 255, 255, 255, 255);
			DrawLine(screen[4].x, screen[4].y, screen[3].x, screen[3].y, 255, 255, 255, 255);

			DrawLine(screen[3].x, screen[3].y, screen[9].x, screen[9].y, 255, 255, 255, 255);
			DrawLine(screen[9].x, screen[9].y, screen[11].x, screen[11].y, 255, 255, 255, 255);
			DrawLine(screen[13].x, screen[13].y, screen[11].x, screen[11].y, 255, 255, 255, 255);

			DrawLine(screen[3].x, screen[3].y, screen[8].x, screen[8].y, 255, 255, 255, 255);
			DrawLine(screen[10].x, screen[10].y, screen[8].x, screen[8].y, 255, 255, 255, 255);
			DrawLine(screen[10].x, screen[10].y, screen[12].x, screen[12].y, 255, 255, 255, 255);

			DrawLine(screen[18].x, screen[18].y, screen[7].x, screen[7].y, 255, 255, 255, 255);
			DrawLine(screen[18].x, screen[18].y, screen[19].x, screen[19].y, 255, 255, 255, 255);
			DrawLine(screen[15].x, screen[15].y, screen[19].x, screen[19].y, 255, 255, 255, 255);

			DrawLine(screen[16].x, screen[16].y, screen[7].x, screen[7].y, 255, 255, 255, 255);
			DrawLine(screen[16].x, screen[16].y, screen[17].x, screen[17].y, 255, 255, 255, 255);
			DrawLine(screen[14].x, screen[14].y, screen[17].x, screen[17].y, 255, 255, 255, 255);
		}
	}
}

void Chatspammer(CUserCmd* cmd)
{
	// Don't spam it too fast, otherwise the thread will lock up
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.5)
		return;

	char buffer[256];

	sprintf_s(buffer, (char*)random[rand() % 19 + 1].c_str());
	SayInChat(buffer);


	start_t = clock();
}

void Bunnyhop(CUserCmd* cmd)
{
	if (cmd->buttons & IN_JUMP && !(Me()->GetFlags() & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;
}

void DoAntiAim(CUserCmd* cmd)
{
	if (!Interface.Settings->antiaim)
		return;

	if (Interface.Settings->backjitter)
		BackJitter(cmd);
	if (Interface.Settings->fastspin)
		FastSpin(cmd);
	if (Interface.Settings->pitchup)
		PitchUp(cmd);
	if (Interface.Settings->flip)
		Flip(cmd);
}

void CL_FixMove(CUserCmd* cmd, Vector viewangles)
{
	Vector move, movenorm, dir, set;
	Vector movenormang, adjusted;

	float len;

	move.Init(cmd->forwardmove, cmd->sidemove, cmd->upmove);

	movenorm = move;

	Math::VectorNormalize(movenorm);

	len = move.Length();

	Math::VectorAngles(movenorm, movenormang);

	if ((cmd->viewangles.x >= 89.0f) || (cmd->viewangles.x <= -89.0f))
	{
		if (viewangles.x >= 0.0f && viewangles.x <= 89.0f)
		{
			viewangles.x = cmd->viewangles.x + 180.0f;
		}
		if (viewangles.x <= 0.0f && viewangles.x >= -89.0f)
		{
			viewangles.x = cmd->viewangles.x - 180.0f;
		}
	}

	adjusted = movenormang + (cmd->viewangles - viewangles);

	Math::AngleVectors(adjusted, &dir);

	set = dir * len;

	if ((cmd->viewangles.x > 89.0f) || (cmd->viewangles.x < -89.0f))
		cmd->forwardmove = set.x;
	else if ((cmd->viewangles.x == 89.0f || cmd->viewangles.x == -89.0f))
		cmd->forwardmove = -set.x;
	else
		cmd->forwardmove = set.x;

	if ((cmd->viewangles.x >= 89.0f) || (cmd->viewangles.x <= -89.0f))
		cmd->sidemove = -set.y;
	else
		cmd->sidemove = set.y;

	if (cmd->sidemove < -450)
		cmd->sidemove = -450;
	if (cmd->sidemove > 450)
		cmd->sidemove = 450;
	if (cmd->forwardmove < -450)
		cmd->forwardmove = -450;
	if (cmd->forwardmove > 450)
		cmd->forwardmove = 450;
}

void __stdcall new_PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	typedef void(__stdcall* PaintTraverse_t)(unsigned int, bool, bool);
	Interface.PanelVMT->Function<PaintTraverse_t>(41)(vguiPanel, forceRepaint, allowForce);

	std::string szPanelName = Interface.Panel->GetName(vguiPanel);

	if (!szPanelName.empty() && !szPanelName.compare("FocusOverlayPanel"))//panel which doesn't get fucked by cl_showpos or whatever
	{
		Interface.EngineClient->GetScreenSize(ScreenSize2.Width, ScreenSize2.Height);

		if (ScreenSize2.Width != ScreenSize.Width || ScreenSize2.Height != ScreenSize.Height)//rebuild font when res changed
		{
			Font = Interface.Surface->CreateFont();
			Interface.Surface->SetFontGlyphSet(Font, "Comic Sans MS", 14, 0, 0, 0, FONTFLAG_OUTLINE);
			ScreenSize.Width = ScreenSize2.Width;
			ScreenSize.Height = ScreenSize2.Height;

		}

		if (Interface.Settings->valvetag)
		{
			SetClanTag("[VALV\xE1\xB4\xB1]", "Valve");
		}

		if (Interface.Settings->envytag)
		{
			SetClanTag("ENVYUS", "EnVyUs");
		}

		if (Interface.Settings->danktag)
		{
			SetClanTag("Pro", "Pro");
		}

		if (!Interface.EngineClient->IsTakingScreenshot())
		{
			ESP();

			if (Interface.Settings->rdot)
				RecoilDot();
		}

		static auto MenuActive = false;

		if (GetAsyncKeyState(VK_INSERT) & 1)
			MenuActive = !MenuActive;

		if (MenuActive)
		{
			Interface.Menu->InsertMenuItems();
			Interface.Menu->DrawMenu();
		}
	}
}

void __stdcall new_CreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	if (Interface.Input == nullptr) return;

	CUserCmd* cmd = Interface.Input->GetUserCmd(sequence_number);
	CVerifiedUserCmd* vcmd = Interface.Input->GetVerifiedUserCmd(sequence_number);

	if (!Me()) return;

	CEntity* Weapon = Me()->GetBaseCombatWeapon();

	if (!Weapon) return;

	Vector vecOldView = cmd->viewangles;

	if (Me()->IsShootableWeapon())
	{
		RunPred(cmd);

		if (Interface.Settings->aim)
			Aimbot(cmd);

		if (Interface.Settings->antiaim)
		{
			DoAntiAim(cmd);
		}

		float flCurTime = Me()->GetTickBase() * Interface.GlobalVars->interval_per_tick;
		float flNextAttack = Me()->GetNextPrimaryAttack();

		if (Interface.Settings->autopistol == true && flNextAttack > flCurTime)
		{
			cmd->buttons &= ~IN_ATTACK;
		}
	}

	if (Interface.Settings->bhop)
	{
		Bunnyhop(cmd);
	}

	if (Interface.Settings->chatspam)
	{
		Chatspammer(cmd);
	}

	//anti angle untrust
	Math::ClampAngles(cmd->viewangles);

	//fix movement with cmd->viewangles and original view otherwise movement will be fucked if silent aim or antiaim is active
	CL_FixMove(cmd, vecOldView);

	vcmd->m_cmd = *cmd;
	vcmd->m_crc = cmd->GetChecksum();
}
