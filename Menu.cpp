#include "Menu.h"

KeyStroke_t m_KeyStroke[KEY_MAX] =
{
	{ "", "", "" }, // 0: KEY_NONE
	{ "", "", "" }, // 1: KEY_MOUSE_LEFT
	{ "", "", "" }, // 2: KEY_MOUSE_RIGHT
	{ "", "", "" }, // 3: KEY_MOUSE_INVALID
	{ "", "", "" }, // 4: KEY_MOUSE_MIDDLE
	{ "", "", "" }, // 5: KEY_MOUSE_4
	{ "", "", "" }, // 6: KEY_MOUSE_5
	{ "", "", "" }, // 7:
	{ "", "", "" }, // 8: KEY_BACKSPACE
	{ "", "", "" }, // 9:
	{ "", "", "" }, // 10:
	{ "", "", "" }, // 11:
	{ "", "", "" }, // 12:
	{ "", "", "" }, // 13: KEY_ENTER
	{ "", "", "" }, // 14:
	{ "", "", "" }, // 15:
	{ "", "", "" }, // 16: KEY_SHIFT
	{ "", "", "" }, // 17:
	{ "", "", "" }, // 18:
	{ "", "", "" }, // 19:
	{ "", "", "" }, // 20: KEY_CAPSTOGGLE
	{ "", "", "" }, // 21:
	{ "", "", "" }, // 22:
	{ "", "", "" }, // 23:
	{ "", "", "" }, // 24:
	{ "", "", "" }, // 25:
	{ "", "", "" }, // 26:
	{ "", "", "" }, // 27: KEY_ESCAPE
	{ "", "", "" }, // 28:
	{ "", "", "" }, // 29:
	{ "", "", "" }, // 30:
	{ "", "", "" }, // 31:
	{ " ", " ", " " }, // 32: KEY_SPACE
	{ "", "", "" }, // 33: KEY_PAGE_UP
	{ "", "", "" }, // 34: KEY_PAGE_DOWN
	{ "", "", "" }, // 35: KEY_END
	{ "", "", "" }, // 36: KEY_POS1
	{ "", "", "" }, // 37: KEY_ARROW_LEFT
	{ "", "", "" }, // 38: KEY_ARROW_UP
	{ "", "", "" }, // 39: KEY_ARROW_RIGHT
	{ "", "", "" }, // 40: KEY_ARROW_DOWN
	{ "", "", "" }, // 41:
	{ "", "", "" }, // 42:
	{ "", "", "" }, // 43:
	{ "", "", "" }, // 44:
	{ "", "", "" }, // 45: KEY_INSERT
	{ "", "", "" }, // 46: KEY_DELETE
	{ "", "", "" }, // 47:
	{ "0", "=", "}" }, // 48: KEY_0
	{ "1", "!", "" }, // 49: KEY_1
	{ "2", "'", "²" }, // 50: KEY_2
	{ "3", "§", "³" }, // 51: KEY_3
	{ "4", "$", "" }, // 52: KEY_4
	{ "5", "%", "" }, // 53: KEY_5
	{ "6", "&", "" }, // 54: KEY_6
	{ "7", "/", "{" }, // 55: KEY_7
	{ "8", "(", "[" }, // 56: KEY_8
	{ "9", ")", "]" }, // 57: KEY_9
	{ "", "", "" }, // 58:
	{ "", "", "" }, // 59:
	{ "", "", "" }, // 60:
	{ "", "", "" }, // 61:
	{ "", "", "" }, // 62:
	{ "", "", "" }, // 63:
	{ "", "", "" }, // 64:
	{ "a", "A", "" }, // 65: KEY_A
	{ "b", "B", "" }, // 66: KEY_B
	{ "c", "C", "" }, // 67: KEY_C
	{ "d", "D", "" }, // 68: KEY_D
	{ "e", "E", "€" }, // 69: KEY_E
	{ "f", "F", "" }, // 70: KEY_F
	{ "g", "G", "" }, // 71: KEY_G
	{ "h", "H", "" }, // 72: KEY_H
	{ "i", "I", "" }, // 73: KEY_I
	{ "j", "J", "" }, // 74: KEY_J
	{ "k", "K", "" }, // 75: KEY_K
	{ "l", "L", "" }, // 76: KEY_L
	{ "m", "M", "µ" }, // 77: KEY_M
	{ "n", "N", "" }, // 78: KEY_N
	{ "o", "O", "" }, // 79: KEY_O
	{ "p", "P", "" }, // 80: KEY_P
	{ "q", "Q", "@" }, // 81: KEY_Q
	{ "r", "R", "" }, // 82: KEY_R
	{ "s", "S", "" }, // 83: KEY_S
	{ "t", "T", "" }, // 84: KEY_T
	{ "u", "U", "" }, // 85: KEY_U
	{ "v", "V", "" }, // 86: KEY_V
	{ "w", "W", "" }, // 87: KEY_W
	{ "x", "X", "" }, // 88: KEY_X
	{ "y", "Y", "" }, // 89: KEY_Y
	{ "z", "Z", "" }, // 90: KEY_Z
	{ "", "", "" }, // 91: KEY_RWIN
	{ "", "", "" }, // 92: KEY_LWIN
	{ "", "", "" }, // 93: KEY_SETTINGS
	{ "", "", "" }, // 94:
	{ "", "", "" }, // 95:
	{ "0", "", "" }, // 96: KEY_NUMPAD_0
	{ "1", "", "" }, // 97: KEY_NUMPAD_1
	{ "2", "", "" }, // 98: KEY_NUMPAD_2
	{ "3", "", "" }, // 99: KEY_NUMPAD_3
	{ "4", "", "" }, // 100: KEY_NUMPAD_4
	{ "5", "", "" }, // 101: KEY_NUMPAD_5
	{ "6", "", "" }, // 102: KEY_NUMPAD_6
	{ "7", "", "" }, // 103: KEY_NUMPAD_7
	{ "8", "", "" }, // 104: KEY_NUMPAD_8
	{ "9", "", "" }, // 105: KEY_NUMPAD_9
	{ "*", "", "" }, // 106: KEY_NUMPAD_MULTIPLICATE
	{ "+", "", "" }, // 107: KEY_NUMPAD_PLUS
	{ "", "", "" }, // 108:
	{ "-", "", "" }, // 109: KEY_NUMPAD_MINUS
	{ "", "", "" }, // 110:
	{ "/", "", "" }, // 111: KEY_NUMPAD_DIVIDE
	{ "", "", "" }, // 112: KEY_F1
	{ "", "", "" }, // 113: KEY_F2
	{ "", "", "" }, // 114: KEY_F3
	{ "", "", "" }, // 115: KEY_F4
	{ "", "", "" }, // 116: KEY_F5
	{ "", "", "" }, // 117: KEY_F6
	{ "", "", "" }, // 118: KEY_F7
	{ "", "", "" }, // 119: KEY_F8
	{ "", "", "" }, // 120: KEY_F9
	{ "", "", "" }, // 121: KEY_F10
	{ "", "", "" }, // 122: KEY_F11
	{ "", "", "" }, // 123: KEY_F12
	{ "", "", "" }, // 124:
	{ "", "", "" }, // 125:
	{ "", "", "" }, // 126:
	{ "", "", "" }, // 127:
	{ "", "", "" }, // 128:
	{ "", "", "" }, // 129:
	{ "", "", "" }, // 130:
	{ "", "", "" }, // 131:
	{ "", "", "" }, // 132:
	{ "", "", "" }, // 133:
	{ "", "", "" }, // 134:
	{ "", "", "" }, // 135:
	{ "", "", "" }, // 136:
	{ "", "", "" }, // 137:
	{ "", "", "" }, // 138:
	{ "", "", "" }, // 139:
	{ "", "", "" }, // 140:
	{ "", "", "" }, // 141:
	{ "", "", "" }, // 142:
	{ "", "", "" }, // 143:
	{ "", "", "" }, // 144: KEY_NUMPAD_NUM
	{ "", "", "" }, // 145:
	{ "", "", "" }, // 146:
	{ "", "", "" }, // 147:
	{ "", "", "" }, // 148:
	{ "", "", "" }, // 149:
	{ "", "", "" }, // 150:
	{ "", "", "" }, // 151:
	{ "", "", "" }, // 152:
	{ "", "", "" }, // 153:
	{ "", "", "" }, // 154:
	{ "", "", "" }, // 155:
	{ "", "", "" }, // 156:
	{ "", "", "" }, // 157:
	{ "", "", "" }, // 158:
	{ "", "", "" }, // 159:
	{ "", "", "" }, // 160: KEY_LSHIFT
	{ "", "", "" }, // 161: KEY_RSHIFT
	{ "", "", "" }, // 162: KEY_LSTRG
	{ "", "", "" }, // 163: KEY_RSTRG
	{ "", "", "" }, // 164: KEY_ALT
	{ "", "", "" }, // 165: KEY_ALTGR
	{ "", "", "" }, // 166:
	{ "", "", "" }, // 167:
	{ "", "", "" }, // 168:
	{ "", "", "" }, // 169:
	{ "", "", "" }, // 170:
	{ "", "", "" }, // 171:
	{ "", "", "" }, // 172:
	{ "", "", "" }, // 173: KEY_MUTE
	{ "", "", "" }, // 174:
	{ "", "", "" }, // 175:
	{ "", "", "" }, // 176:
	{ "", "", "" }, // 177:
	{ "", "", "" }, // 178:
	{ "", "", "" }, // 179:
	{ "", "", "" }, // 180:
	{ "", "", "" }, // 181:
	{ "", "", "" }, // 182:
	{ "", "", "" }, // 183:
	{ "", "", "" }, // 184:
	{ "", "", "" }, // 185:
	{ "", "", "" }, // 186: KEY_Ü
	{ "+", "*", "~" }, // 187: KEY_PLUS
	{ ",", ";", "" }, // 188: KEY_COMMA
	{ "-", "_", "" }, // 189: KEY_MINUS
	{ ".", ":", "" }, // 190: KEY_DOT
	{ "#", "'", "" }, // 191: KEY_RUE
	{ "", "", "" }, // 192: KEY_Ö
	{ "", "", "" }, // 193:
	{ "", "", "" }, // 194:
	{ "", "", "" }, // 195:
	{ "", "", "" }, // 196:
	{ "", "", "" }, // 197:
	{ "", "", "" }, // 198:
	{ "", "", "" }, // 199:
	{ "", "", "" }, // 200:
	{ "", "", "" }, // 201:
	{ "", "", "" }, // 202:
	{ "", "", "" }, // 203:
	{ "", "", "" }, // 204:
	{ "", "", "" }, // 205:
	{ "", "", "" }, // 206:
	{ "", "", "" }, // 207:
	{ "", "", "" }, // 208:
	{ "", "", "" }, // 209:
	{ "", "", "" }, // 210:
	{ "", "", "" }, // 211:
	{ "", "", "" }, // 212:
	{ "", "", "" }, // 213:
	{ "", "", "" }, // 214:
	{ "", "", "" }, // 215:
	{ "", "", "" }, // 216:
	{ "", "", "" }, // 217:
	{ "", "", "" }, // 218:
	{ "sz", "?", "\\" }, // 219: KEY_BACKSLASH
	{ "^", "°", "" }, // 220: KEY_ROOF
	{ "´", "`", "" }, // 221: KEY_IRGENDWAS
	{ "", "", "" }, // 222: KEY_Ä
	{ "", "", "" }, // 223:
	{ "", "", "" }, // 224:
	{ "", "", "" }, // 225:
	{ "", "", "" }, // 226:
	{ "", "", "" }, // 227:
	{ "", "", "" }, // 228:
	{ "", "", "" }, // 229:
	{ "", "", "" }, // 230:
	{ "", "", "" }, // 231:
	{ "", "", "" }, // 232:
	{ "", "", "" }, // 233:
	{ "", "", "" }, // 234:
	{ "", "", "" }, // 235:
	{ "", "", "" }, // 236:
	{ "", "", "" }, // 237:
	{ "", "", "" }, // 238:
	{ "", "", "" }, // 239:
	{ "", "", "" }, // 240:
	{ "", "", "" }, // 241:
	{ "", "", "" }, // 242:
	{ "", "", "" }, // 243:
	{ "", "", "" }, // 244:
	{ "", "", "" }, // 245:
	{ "", "", "" }, // 246:
	{ "", "", "" }, // 247:
	{ "", "", "" }, // 248:
	{ "", "", "" }, // 249:
	{ "", "", "" }, // 250:
	{ "", "", "" }, // 251:
	{ "", "", "" }, // 252:
	{ "", "", "" }, // 253:
	{ "", "", "" }, // 254:
};

sMenuItem Items[120];
bool AimbotTab = false, TriggerbotTab = false, ESPTab = false, MiscTab = false, AntiAimTab = false, FunStuffTab = false, ClanTagTab = false;
int TotalItems = 0, CurrentPos = 0, MenuX = 40, MenuY = 40;

int AddMenuEntry(int n, std::string name, bool* value)
{
	Items[n].m_Name = name;
	Items[n].m_Bool = value;
	Items[n].m_Type = 0;

	return (n + 1);
}

int AddMenuEntry(int n, std::string name, int* value, int min, int max, int step)
{
	Items[n].m_Name = name;
	Items[n].m_Int = value;
	Items[n].m_IntMin = min;
	Items[n].m_IntMax = max;
	Items[n].m_IntStep = step;
	Items[n].m_Type = 1;

	return (n + 1);
}

int AddMenuEntry(int n, std::string name, float* value, float min, float max, float step)
{
	Items[n].m_Name = name;
	Items[n].m_Float = value;
	Items[n].m_FloatMin = min;
	Items[n].m_FloatMax = max;
	Items[n].m_FloatStep = step;
	Items[n].m_Type = 2;

	return (n + 1);
}

void CMenu::InsertMenuItems()
{
//modify float value template
//i = AddMenuEntry(i, "-Float Value to Change ", &Interface.Settings->fov, min 60, max 360, step 25);
	int i = 0;
	i = AddMenuEntry(i, "Rage Enabled", &Interface.Settings->ragemode);
	if (!AimbotTab)
		i = AddMenuEntry(i, "[+] Aim Settings", &AimbotTab);

	if (AimbotTab)
	{
		i = AddMenuEntry(i, "[-] Aim Settings", &AimbotTab);


		i = AddMenuEntry(i, "-Aimbot Enabled", &Interface.Settings->aim);
		i = AddMenuEntry(i, "-Triggerbot Enabled", &Interface.Settings->trigger);
		i = AddMenuEntry(i, "-SilentAim Enabled", &Interface.Settings->silentaim);
		i = AddMenuEntry(i, "-No Recoil", &Interface.Settings->norecoil);
		i = AddMenuEntry(i, "-AutoPistol Enabled", &Interface.Settings->autopistol);
		i = AddMenuEntry(i, "-Aim Bone = ", &Interface.Settings->bone, 0, 30, 1);
	}

	if (!ESPTab)
		i = AddMenuEntry(i, "[+] ESP Settings", &ESPTab);

	if (ESPTab)
	{
		i = AddMenuEntry(i, "[-] ESP Settings", &ESPTab);

		i = AddMenuEntry(i, "-ESP Enabled", &Interface.Settings->esp);
		i = AddMenuEntry(i, "-Box ESP", &Interface.Settings->box);
		i = AddMenuEntry(i, "-Name ESP", &Interface.Settings->name);
		i = AddMenuEntry(i, "-Health ESP", &Interface.Settings->healthbar);
	}

	if (!AntiAimTab)
	{
		i = AddMenuEntry(i, "[+] Antiaim Settings", &AntiAimTab);
	}
	if (AntiAimTab)
	{
		//not in use 
		//i = AddMenuEntry(i, "-Slow Spin AA Enabled", &Interface.Settings->slowspin);
		//i = AddMenuEntry(i, "-Emotion AA Enabled", &Interface.Settings->emotion);
		//i = AddMenuEntry(i, "-180 Yaw AA Enabled", &Interface.Settings->yawaa);
		//i = AddMenuEntry(i, "-Fake Sideways AA Enabled", &Interface.Settings->FakeSideways);
		i = AddMenuEntry(i, "[-] Antiaim Settings", &AntiAimTab);
		i = AddMenuEntry(i, "-Antiaim Enabled", &Interface.Settings->antiaim);
		i = AddMenuEntry(i, "-Back Jitter AA Enabled", &Interface.Settings->backjitter);
		i = AddMenuEntry(i, "-Flip AA Enabled", &Interface.Settings->flip);
		i = AddMenuEntry(i, "-Fast Spin AA Enabled", &Interface.Settings->fastspin);

	}
	if (!FunStuffTab)
	{
		i = AddMenuEntry(i, "[+] Fun Stuff", &FunStuffTab);
	}

	if (FunStuffTab)
	{
		//not in use
		//i = AddMenuEntry(i, "-Report Spam Enabled", &Interface.Settings->reportspam);
		//i = AddMenuEntry(i, "-Fakelag Enabled", &Interface.Settings->fakelag);

		i = AddMenuEntry(i, "[-] Fun Stuff", &FunStuffTab);

		i = AddMenuEntry(i, "-Chatspammer Enabled", &Interface.Settings->chatspam);
		i = AddMenuEntry(i, "-Bunnyhop Enabled", &Interface.Settings->bhop);

	}
	
	if (!ClanTagTab)
	{
		i = AddMenuEntry(i, "[+] Clan Tags", &ClanTagTab);
	}

	if (ClanTagTab)
	{
		i = AddMenuEntry(i, "[-] Clan Tags", &ClanTagTab);
		i = AddMenuEntry(i, "-ValvE Clan Tag Enabled", &Interface.Settings->valvetag);
		i = AddMenuEntry(i, "-ENVYUS Tag Enabled", &Interface.Settings->envytag);
		i = AddMenuEntry(i, "-Pro Tag Enabled", &Interface.Settings->danktag);

	}

	TotalItems = i;
}

void CMenu::DrawMenu()
{
	int FontSize = 14;

	//FillRGBA(MenuX - 20, MenuY + 1 + (FontSize * CurrentPos) + 4, 215, FontSize + 2, 255, 255, 255, 255);
	FillRGBA(MenuX - 20, MenuY + 2 + (FontSize * CurrentPos) + 3, 215, FontSize + 2, 255, 255, 255, 255);

	for (int i = 0; i < TotalItems; ++i)
	{
		if (!strncmp(Items[i].m_Name.c_str(), "[+]", 3) || !strncmp(Items[i].m_Name.c_str(), "[-]", 3)) // ghetto tab check
			Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, Items[i].m_Name.c_str());
		else{
			switch (Items[i].m_Type)
			{
			case 0:
				Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, Items[i].m_Name.c_str());
				Text(MenuX + 150, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, "%s", (*Items[i].m_Bool) ? "true" : "false");
				break;
			case 1:
				Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, Items[i].m_Name.c_str());
				Text(MenuX + 150, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, "%i", *Items[i].m_Int);
				break;
			case 2:
				Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, Items[i].m_Name.c_str());
				Text(MenuX + 150, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, "%.2f", *Items[i].m_Float);
				break;
			case 3: 
			//	Text(MenuX, MenuY + 2 + (FontSize * i) + 4, 255, 255, 255, 255, 0, Font, displayText.c_str());
				break;
			default:
				break;
			}
		}
	}

	if (GetAsyncKeyState(VK_UP) & 1)
	{
		if (CurrentPos > 0)
			CurrentPos--;
		else
			CurrentPos = TotalItems - 1;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 1)
	{
		if (CurrentPos < TotalItems - 1)
			CurrentPos++;
		else
			CurrentPos = 0;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 1)
	{
		switch (Items[CurrentPos].m_Type)
		{
		case 0:
			*Items[CurrentPos].m_Bool = !*Items[CurrentPos].m_Bool;
			break;
		case 1:
			*Items[CurrentPos].m_Int -= Items[CurrentPos].m_IntStep;
			if (*Items[CurrentPos].m_Int < Items[CurrentPos].m_IntMin)
				*Items[CurrentPos].m_Int = Items[CurrentPos].m_IntMax;
			break;
		case 2:
			*Items[CurrentPos].m_Float -= Items[CurrentPos].m_FloatStep;
			if (*Items[CurrentPos].m_Float < Items[CurrentPos].m_FloatMin)
				*Items[CurrentPos].m_Float = Items[CurrentPos].m_FloatMax;
			break;
		default:
			break;
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 1)
	{
		switch (Items[CurrentPos].m_Type)
		{
		case 0:
			*Items[CurrentPos].m_Bool = !*Items[CurrentPos].m_Bool;
			break;
		case 1:
			*Items[CurrentPos].m_Int += Items[CurrentPos].m_IntStep;
			if (*Items[CurrentPos].m_Int > Items[CurrentPos].m_IntMax)
				*Items[CurrentPos].m_Int = Items[CurrentPos].m_IntMin;
			break;
		case 2:
			*Items[CurrentPos].m_Float += Items[CurrentPos].m_FloatStep;
			if (*Items[CurrentPos].m_Float > Items[CurrentPos].m_FloatMax)
				*Items[CurrentPos].m_Float = Items[CurrentPos].m_FloatMin;
			break;
		default:
			break;
		}
	}
}

