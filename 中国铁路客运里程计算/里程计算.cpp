#include<stdio.h>
#include<io.h>
#include<Windows.h>
#include<conio.h>
#include<ctype.h>
#include<Commdlg.h>
#include<thread>
#include<ShlObj.h>
#include"data.h"
#include"Time.h"
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4477)
#pragma warning(disable:6001)
#pragma warning(disable:6011)
constexpr int HengYang = 0x0001;
constexpr int XiNing = 0x0002;
typedef struct structure1 {
	line* thisline;
	int start;
	int end;
	struct structure1* prime;
	struct structure1* next;
}way;
typedef struct structure2 {
	short int last;
	short int line;
	short int Kms;
}step;
COORD H1_CursorPosition() {
	CONSOLE_SCREEN_BUFFER_INFO pBuffer;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pBuffer);
	return pBuffer.dwCursorPosition;
}
void H2_Initilize() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	system("mode con cols=120 lines=30");
	SetConsoleTitleA("中国铁路客运里程查询");
	CONSOLE_CURSOR_INFO t = { FALSE,1 };
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &t);
	system("cls");
	return;
}
int H3_SubString(const char base[], const char string[]) {
	int a = strlen(base), b = strlen(string);
	if (a <= 0 || b <= 0 || a > b)return -1;
	for (int i = 0; i <= b - a; i++) {
		for (int j = 0; j < a; j++) {
			if (base[j] != string[i + j])break;
			if (j == a - 1)return i;
		}
		if (base[0] < 0)i++;
	}
	return -1;
}
void H4_StationSearch(const char str[],int a[], int length) {
	if (str[0] == 0)return;
	if (str[0] > 0) {
		int t = 0, temp[300] = { 0 }, count = 0;
		for (int i = 0; i < STATION_SIZE; i++) {
			if ((t=H3_SubString(str, STATION[i].Spell)) == 0) {
				a[0] = i + 1;
				a++;
				length--;
				if (length <= count)break;
			}
			else if (t > 0) {
				if (count < 300)temp[count++] = i + 1;
			}
		}
		for (int i = 0; i < count; i++)*a++ = temp[i];
		return;
	}
	if (str[0] < 0) {
		int t = 0, temp[300] = { 0 }, count = 0;
		for (int i = 0; i < STATION_SIZE; i++) {
			if ((t = H3_SubString(str, STATION[i].Name)) == 0) {
				a[0] = i + 1;
				a++;
				length--;
				if (length <= count)break;
			}
			else if (t > 0) {
				if (count < 300)temp[count++] = i + 1;
			}
		}
		for (int i = 0; i < count; i++)*a++ = temp[i];
		return;
	}
}
int H5_StationLocate(int st, int a[], int length, int mode) {
	int t = 0;
	for (int i = 0; i < LINE_SIZE; i++) {
		for (int j = 0; j < LINE[i].StationNumbers; j++) {
			if (LINE[i].StationList[j].Station == st) {
				if (mode == 1)return i;
				a[t] = i;
				t++;
				if (t == length)return 0;
				break;
			}
		}
	}
	return 0;
}
void H6_gotoxy(int x, int y) {
	COORD coord = { (short)x,(short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}
int H7_StationList(int list[], int length, int mode, COORD coord, int line) {
	static int pos = 0, start = 0;
	if (mode == 3)pos = start = 0;
	if (mode == 2)return list[start + pos];
	coord.Y++;
	coord.X = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	if (mode == -2) {
		H6_gotoxy(coord.X, coord.Y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		for (int j = 0; j < 21; j++) {
			putchar('\n');
			for (int k = 0; k < 40; k++)putchar(' ');
		}
		return -2;
	}
	if (mode == -1) {
		if (pos == 0 && start != 0)start--;
		else if (pos != 0)pos--;
	}
	if (mode == 1) {
		if (pos == 19 && start + pos < length - 1)start++;
		else if (pos < 19 && start + pos < length - 1)pos++;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 55);
	if (mode == 0) {
		for (int i = printf("未检索到任何结果"); i < 40; i++)putchar(' ');
		for (int j = 0; j < 20; j++) {
			H6_gotoxy(coord.X, coord.Y + j + 1);
			for (int k = 0; k < 40; k++)putchar(' ');
		}
		start = pos = 0;
		return 0;
	}
	H6_gotoxy(coord.X, coord.Y);
	if (length > 0)for (int i = printf("查询结果：%d/%d", pos + start + 1, length); i < 40; i++)putchar(' ');
	else {
		for (int i = printf("未检索到任何结果"); i < 40; i++)putchar(' ');
		for (int j = 0; j < 20; j++) {
			H6_gotoxy(coord.X, coord.Y + j + 1);
			for (int k = 0; k < 40; k++)putchar(' ');
		}
		return -1;
	}
	putchar('\n');
	if (mode == 0) {
		for (int j = 0; j < 20; j++) {
			H6_gotoxy(coord.X, coord.Y + j + 1);
			for (int k = 0; k < 40; k++)putchar(' ');
		}
		start = pos = 0;
		return 0;
	}
	for (int i = start; i < start + 20 && i < length; i++) {
		station st = STATION[list[i] - 1];
		H6_gotoxy(coord.X, coord.Y + i - start + 1);
		int j = 0;
		if (line == -1) {
			j = printf("%s（%s，%s", st.Name, PROVINCE[st.Province - 1].Name, LINE[H5_StationLocate(list[i], NULL, 0, 1)].Name);
			if (st.Freq > 1)j += printf("等）");
			else j += printf("）");
		}
		else {
			int t[2] = { -1,-1 };
			H5_StationLocate(list[i], t, 2, 0);
			if (t[0] == line)t[0] = t[1];
			j = printf("%s（%s，%s", st.Name, PROVINCE[st.Province - 1].Name, t[0] == -1 ? "终到站" : LINE[t[0]].Name);
			if (st.Freq > 2 && t[0] != -1)j += printf("等）");
			else j += printf("）");
		}
		for (; j < 40; j++)putchar(' ');
	}

	int t = start + pos;
	if (pos != 19) {
		if (start == 0)coord.Y += t + 1;
		else if (start != 0)coord.Y += pos + 1;
	}
	else coord.Y += 20;
	H6_gotoxy(coord.X, coord.Y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 63);
	station st = STATION[list[t] - 1];
	int j = 0;
		if (line == -1) {
			j = printf("%s（%s，%s", st.Name, PROVINCE[st.Province - 1].Name, LINE[H5_StationLocate(list[t], NULL, 0, 1)].Name);
			if (st.Freq > 1)j += printf("等）");
			else j += printf("）");
		}
		else {
			int rt[2] = { -1,-1 };
			H5_StationLocate(list[t], rt, 2, 0);
			if (rt[0] == line)rt[0] = rt[1];
			j = printf("%s（%s，%s", st.Name, PROVINCE[st.Province - 1].Name, rt[0] == -1 ? "终到站" : LINE[rt[0]].Name);
			if (st.Freq > 2 && rt[0] != -1)j += printf("等）");
			else j += printf("）");
		}
	for (; j < 40; j++)putchar(' ');
	return 0;
}
int H8_LineList(int list[], int length, int mode, COORD coord) {
	static int pos = 0, start = 0;
	if (mode == 3)pos = start = 0;
	if (mode == 2)return list[start + pos];
	coord.Y++;
	coord.X = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	if (mode == -2) {
		H6_gotoxy(coord.X, coord.Y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		for (int j = 0; j < 21; j++) {
			for (int k = 0; k < 60; k++)putchar(' ');
			putchar('\n');
		}
		return -2;
	}
	if (mode == -1) {
		if (pos == 0 && start != 0)start--;
		else if (pos != 0)pos--;
	}
	if (mode == 1) {
		if (pos == 19 && start + pos < length - 1)start++;
		else if (pos < 19 && start + pos < length - 1)pos++;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 55);
	H6_gotoxy(coord.X, coord.Y);
	if (mode == 0) {
		for (int i = printf("未检索到任何结果"); i < 60; i++)putchar(' ');
		for (int j = 0; j < 20; j++) {
			H6_gotoxy(coord.X, coord.Y + j + 1);
			for (int k = 0; k < 60; k++)putchar(' ');
		}
		start = pos = 0;
		return 0;
	}
	if (length > 0)for (int i = printf("查询结果：%d/%d", pos + start + 1, length); i < 60; i++)putchar(' ');
	else {
		for (int i = printf("未检索到任何结果"); i < 60; i++)putchar(' ');
		for (int j = 0; j < 20; j++) {
			H6_gotoxy(coord.X, coord.Y + j + 1);
			for (int k = 0; k < 60; k++)putchar(' ');
		}
		return -1;
	}
	putchar('\n');
	if (mode == 0) {
		for (int j = 0; j < 20; j++) {
			H6_gotoxy(coord.X, coord.Y + j + 1);
			for (int k = 0; k < 60; k++)putchar(' ');
		}
		return 0;
	}
	for (int i = start; i < start + 20 && i < length; i++) {
		line st = LINE[list[i]];
		H6_gotoxy(coord.X, coord.Y + i - start + 1);
		for (int j = printf("%s（%s—%s）", st.Name, STATION[st.StationList[0].Station - 1].Name, STATION[st.StationList[st.StationNumbers - 1].Station - 1].Name); j < 60; j++)putchar(' ');
	}
	int t = start + pos;
	if (pos != 19) {
		if (start == 0)coord.Y += t + 1;
		else if (start != 0)coord.Y += pos + 1;
	}
	else coord.Y += 20;
	H6_gotoxy(coord.X, coord.Y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 63);
	line st = LINE[list[t]];
	for (int j = printf("%s（%s—%s）", st.Name, STATION[st.StationList[0].Station - 1].Name, STATION[st.StationList[st.StationNumbers - 1].Station - 1].Name); j < 60; j++)putchar(' ');
	return 0;
}
int H9_LinkPrint(way* head, int length) {
	if (head == NULL)return 0;
	int count = length - 1;
	int wayl = 0;
	int step = 0;
	int error = 0;
	while (1) {
		
		if (head->next == NULL)break;
		head = head->next;
	}
	while (1) {
		if (head->prime == NULL)break;
		head = head->prime;
		if (--count == 0)break;
	}
	while (1) {
		const char* start = STATION[head->thisline->StationList[head->start].Station - 1].Name;
		const char* end = STATION[head->thisline->StationList[head->end].Station - 1].Name;
		const char* line = head->thisline->Name;
		int skm = head->thisline->StationList[head->start].Kms;
		int ekm = head->thisline->StationList[head->end].Kms;
		char s[60] = { 0 };

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
		sprintf_s(s, sizeof(s), "%s（%s—%s）", head->thisline->Name, STATION[head->thisline->StationList[0].Station - 1].Name, STATION[head->thisline->StationList[head->thisline->StationNumbers - 1].Station - 1].Name);
		if (strlen(s) <= 36) {
			for (int i = (36 - strlen(s)) / 2; i > 0; i--)putchar(' ');
			printf("%s", s);
			for (int i = (36 - strlen(s)) / 2; i > 0; i--)putchar(' ');
		}
		else printf("%s", s);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 63);
		if (strcmp(start + strlen(start) - 2, "所") == 0)sprintf_s(s, sizeof(s), "%s（%dkm）", start, skm);
		else sprintf_s(s, sizeof(s), "%s站（%dkm）", start, skm);
		int t = strlen(s);
		if (t <= 24) {
			if (t != (t / 2) * 2)putchar(' ');
			for (int i = (24 - t) / 2; i > 0; i--)putchar(' ');
			printf("%s", s);
			for (int i = (24 - t) / 2; i > 0; i--)putchar(' ');
		}
		else printf("%s", s);
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("至");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 63);
		if (strcmp(end + strlen(end) - 2, "所") == 0)sprintf_s(s, sizeof(s), "%s（%dkm）", end, ekm);
		else sprintf_s(s, sizeof(s), "%s站（%dkm）", end, ekm);
		t = strlen(s);
		if (t <= 24) {
			if (t != (t / 2) * 2)putchar(' ');
			for (int i = (24 - t) / 2; i > 0; i--)putchar(' ');
			printf("%s", s);
			for (int i = (24 - t) / 2; i > 0; i--)putchar(' ');
		}
		else printf("%s", s);

		step = ekm - skm > 0 ? ekm - skm : skm - ekm;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("里程为：%4dkm", step);
		wayl += step;
		putchar('\n');
		head = head->next;
		if (head == NULL)break;
	}
	return wayl;
}
void H10_LineSearch(const char str[], int a[], int length) {
	if (str[0] == 0)return;
	if (str[0] > 0) {
		int t = 0, temp[32] = { 0 }, count = 0;
		for (int i = 0; i < 32; i++)temp[i] = -1;
		for (int i = 0; i < LINE_SIZE; i++) {
			if ((t = H3_SubString(str, LINE[i].Spell)) == 0) {
				a[0] = i;
				a++;
				length--;
				if (length <= count)break;
			}
			else if (t > 0) {
				if (count < 30)temp[count++] = i;
			}
		}
		for (int i = 0; i < count; i++)*a++ = temp[i];
		return;
	}
	if (str[0] < 0) {
		int t = 0, temp[32] = { 0 }, count = 0;
		for (int i = 0; i < LINE_SIZE; i++) {
			if ((t = H3_SubString(str, LINE[i].Name)) == 0) {
				a[0] = i;
				a++;
				length--;
				if (length <= count)break;
			}
			else if (t > 0) {
				if (count < 32)temp[count++] = i;
			}
		}
		for (int i = 0; i < count; i++)*a++ = temp[i];
		return;
	}
}
void H11_DestroyLinkList(way* head) {
	if (head == NULL)return;
	if (head->next != NULL)H11_DestroyLinkList(head->next);
	free(head);
	return;
}
int H12_StationLine(int t1, int t2, int l) {
	const char* p = LINE[l].Mode_Set_Information;
	if (p == NULL)return 0;
	int s[3] = { 0 }, t[3] = { 0 },  pair = sscanf_s(p, "%*d%d-%d%d-%d%d-%d", s, t, s + 1, t + 1, s + 2, t + 2) / 2;
	if (t2 == 0) {
		for (int i = 0; i < pair; i++) {
			for (int j = s[i] - 1; j <= t[i] - 1; j++) {
				if (LINE[l].StationList[j].Station == t1)return 1;
			}
		}
		return 0;
	}
	for (int i = 0; i < pair; i++) {
		int sc = 0;
		for (int j = s[i] - 1; j <= t[i] - 1; j++) {
			if (LINE[l].StationList[j].Station == t1)sc++;
			if (LINE[l].StationList[j].Station == t2)sc++;
			if (sc == 2)return 1;
		}
	}
	return 0;
}
int H13_StationSearch(const char* base) {
	for (int i = 0; i < STATION_SIZE; i++)if (strcmp(base, STATION[i].Name) == 0)return i + 1;
	return 0;
}
int H14_StationMode(int t) {
	int list[20] = { 0 }, count = 0;
	int np = 0, nh = 0;
	for (int i = 0; i < 20; i++)list[i] = -1;
	H5_StationLocate(t, list, 20, 0);
	for (int i = 0; i < 20; i++)if (list[i] == -1) {
		count = i;
		break;
	}
	for (int i = 0; i < count; i++) {
		if (LINE[list[i]].Mode == 1 || LINE[list[i]].Mode == 3 || (H12_StationLine(t, 0, list[i]) && LINE[list[i]].Mode_Set_Information[0] == '1'))np = 1;
		if (LINE[list[i]].Mode == 2 || LINE[list[i]].Mode == 3 || (H12_StationLine(t, 0, list[i]) && LINE[list[i]].Mode_Set_Information[0] == '2'))nh = 1;
		if (np == 1 && nh == 1)return 3;
	}
	return np + nh * 2;
	return 0;
}
int H15_LastExStation(int line, int station, int mode, int avd[], int avdl) {
	int i = 0, j = 0;
	while (1) {
		if (LINE[line].StationList[i].Station == station) {
			j = i;
			break;
		}
		i++;
		if (i == LINE[line].StationNumbers)return -1;
	}
	while (1) {
		i--;
		if (i == -1)return -1;
		if (avdl)for (int t = 0; t < avdl; t++)if (avd[t] == LINE[line].StationList[i].Station)return -1;
		if (STATION[LINE[line].StationList[i].Station - 1].Freq > 1) {
			if ((mode == 3 && LINE[line].Mode != 0) || LINE[line].Mode == 3)return i;
			if (mode == LINE[line].Mode)return i;
			if (LINE[line].Mode_Set_Information != NULL&& (LINE[line].Mode_Set_Information[0] - '0' == mode || LINE[line].Mode_Set_Information[0] == '3') && H12_StationLine(LINE[line].StationList[i].Station, LINE[line].StationList[j].Station, line) == 1)return i;
			return -1;
		}
	}
}
int H16_NextExStation(int line, int station,int mode,int avd[],int avdl) {
	int i = 0, j = 0;
	while (1) {
		if (LINE[line].StationList[i].Station == station) {
			j = i;
			break;
		}
		i++;
		if (i == LINE[line].StationNumbers)return -1;
	}
	while (1) {
		i++;
		if (i == LINE[line].StationNumbers)return -1;
		if (avdl)for (int t = 0; t < avdl; t++)if (avd[t] == LINE[line].StationList[i].Station)return -1;
		if (STATION[LINE[line].StationList[i].Station - 1].Freq > 1) {
			if ((mode == 3 && LINE[line].Mode != 0) || LINE[line].Mode == 3)return i;
			if (mode == LINE[line].Mode)return i;
			if (LINE[line].Mode_Set_Information != NULL && (LINE[line].Mode_Set_Information[0]-'0' == mode|| LINE[line].Mode_Set_Information[0]=='3')&& H12_StationLine(LINE[line].StationList[i].Station, LINE[line].StationList[j].Station, line) == 1)return i;
			return -1;
		}
	}
}
int H17_LocateStationInLine(int line, int station) {
	int i = 0;
	while (1) {
		if (LINE[line].StationList[i].Station == station)return i;
		i++;
		if (i == LINE[line].StationNumbers)return -1;
	}
}
void H18_WaySort(way*& start, int mode, int linemode, int& error, int avd[], int avdl) {
	way* head = start;
	while (1) {
		if (head == NULL)break;
		if (strcmp("吉衡线", head->thisline->Name) == 0) {
			const char* p1 = STATION[head->thisline->StationList[head->start].Station - 1].Name;
			const char* p2 = STATION[head->thisline->StationList[head->end].Station - 1].Name;
			if ((strcmp(p1, "衡阳北") == 0 || strcmp(p2, "衡阳北") == 0))error |= HengYang;
		}
		if (linemode != 1 && (strcmp("兰新客专线", head->thisline->Name) == 0 || strcmp("兰青线", head->thisline->Name) == 0)) {
			const char* p1 = STATION[head->thisline->StationList[head->start].Station - 1].Name;
			const char* p2 = STATION[head->thisline->StationList[head->end].Station - 1].Name;
			if ((strcmp(p1, "西宁") == 0 || strcmp(p2, "西宁") == 0))error |= XiNing;
		}
		if (head->next != NULL) {
			if (head->thisline == head->next->thisline) {
				head->end = head->next->end;
				way* n = head->next;
				head->next = head->next->next;
				if (head->next != NULL) {
					head->next->prime = head;
				}
				free(n);
				continue;
			}
		}
		head = head->next;
	}
	if (mode == 0)return;
	head = start;
	while (1) {
		if (head == NULL)break;
		if (head->next && head->next->next != NULL) {
			int station = head->next->next->thisline->StationList[head->next->next->end].Station;
			int t = H17_LocateStationInLine(head->thisline - LINE, station);
			if (t != -1) {
				int sete = 0;
				if (avdl) {
					int st = head->start, ed = t;
					if (st > ed) {
						int temp = st;
						st = ed;
						ed = temp;
					}
					for (int r = st; r <= ed; r++) {
						for (int s = 0; s < avdl; s++) {
							if (head->thisline->StationList[r].Station == avd[s]) {
								sete = 1;
								break;
							}
						}
					}
				}
				if (sete == 0) {
					int kms = head->thisline->StationList[t].Kms - head->thisline->StationList[head->end].Kms;
					if (kms < 0)kms *= -1;
					int nextkm = head->next->thisline->StationList[head->next->start].Kms - head->next->thisline->StationList[head->next->end].Kms;
					int tpnextkm = head->next->next->thisline->StationList[head->next->next->start].Kms - head->next->next->thisline->StationList[head->next->next->end].Kms;
					if (nextkm < 0)nextkm *= -1;
					if (tpnextkm < 0)tpnextkm *= -1;
					nextkm += tpnextkm;
					if (kms == nextkm) {
						if ((linemode == 3 && head->thisline->Mode) || (linemode == head->thisline->Mode) || (head->thisline->Mode == head->next->thisline->Mode) || head->thisline->Mode == 3 || H12_StationLine(head->thisline->StationList[t].Station, head->thisline->StationList[head->end].Station, head->thisline - LINE) == 1) {
							head->end = t;
							way* n = head->next, * n2 = n->next;
							head->next = head->next->next->next;
							if (head->next != NULL)head->next->prime = head;
							free(n);
							free(n2);
							continue;
						}
					}
				}
			}
		}
		if (head->next != NULL) {
			int station = head->next->thisline->StationList[head->next->end].Station;
			int t = H17_LocateStationInLine(head->thisline - LINE, station);
			if (t != -1) {
				int sete = 0;
				if (avdl) {
					int st = head->start, ed = t;
					if (st > ed) {
						int temp = st;
						st = ed;
						ed = temp;
					}
					for (int r = st; r <= ed; r++) {
						for (int s = 0; s < avdl; s++) {
							if (head->thisline->StationList[r].Station == avd[s]) {
								sete = 1;
								break;
							}
						}
					}
				}
				if (sete == 0) {
					int kms = head->thisline->StationList[t].Kms - head->thisline->StationList[head->end].Kms;
					if (kms < 0)kms *= -1;
					int nextkm = head->next->thisline->StationList[head->next->start].Kms - head->next->thisline->StationList[head->next->end].Kms;
					if (nextkm < 0)nextkm *= -1;
					if (kms == nextkm) {
						if ((linemode == 3 && head->thisline->Mode) || (linemode == head->thisline->Mode) || (head->thisline->Mode == head->next->thisline->Mode) || head->thisline->Mode == 3 || H12_StationLine(head->thisline->StationList[t].Station, head->thisline->StationList[head->end].Station, head->thisline - LINE) == 1) {
							head->end = t;
							way* n = head->next;
							head->next = head->next->next;
							if (head->next != NULL) {
								head->next->prime = head;
							}
							free(n);
							continue;
						}
					}
				}
			}
		}
		head = head->next;
	}
	head = start;
	while (head->next != NULL)head = head->next;
	while (1) {
		if (head == NULL)break;
		if (head->prime && head->prime->prime!= NULL) {
			int station = head->prime->prime->thisline->StationList[head->prime->prime->start].Station;
			int t = H17_LocateStationInLine(head->thisline - LINE, station);
			if (t != -1) {
				int sete = 0;
				if (avdl) {
					int st = head->start, ed = t;
					if (st > ed) {
						int temp = st;
						st = ed;
						ed = temp;
					}
					for (int r = st; r <= ed; r++) {
						for (int s = 0; s < avdl; s++) {
							if (head->thisline->StationList[r].Station == avd[s]) {
								sete = 1;
								break;
							}
						}
					}
				}
				if (sete == 0) {
					int kms = head->thisline->StationList[t].Kms - head->thisline->StationList[head->start].Kms;
					if (kms < 0)kms *= -1;
					int primekm = head->prime->thisline->StationList[head->prime->start].Kms - head->prime->thisline->StationList[head->prime->end].Kms;
					int pr2km = head->prime->prime->thisline->StationList[head->prime->prime->start].Kms - head->prime->prime->thisline->StationList[head->prime->prime->end].Kms;
					if (primekm < 0)primekm *= -1;
					if (pr2km < 0)pr2km *= -1;
					primekm += pr2km;
					if (kms == primekm) {
						if ((linemode == 3 && head->thisline->Mode) || (linemode == head->thisline->Mode) || (head->thisline->Mode == head->prime->thisline->Mode) || head->thisline->Mode == 3 || H12_StationLine(head->thisline->StationList[t].Station, head->thisline->StationList[head->start].Station, head->thisline - LINE)) {
							head->start = t;
							way* n = head->prime, * n2 = n->prime;
							head->prime = head->prime->prime->prime;
							if (head->prime != NULL) {
								head->prime->next = head;
							}
							else start = head;
							free(n);
							free(n2);
							continue;
						}
					}
				}
			}
		}
		if (head->prime != NULL) {
			int station = head->prime->thisline->StationList[head->prime->start].Station;
			int t = H17_LocateStationInLine(head->thisline - LINE, station);
			if (t != -1) {
				int sete = 0;
				if (avdl) {
					int st = head->start, ed = t;
					if (st > ed) {
						int temp = st;
						st = ed;
						ed = temp;
					}
					for (int r = st; r <= ed; r++) {
						for (int s = 0; s < avdl; s++) {
							if (head->thisline->StationList[r].Station == avd[s]) {
								sete = 1;
								break;
							}
						}
					}
				}
				if (sete == 0) {
					int kms = head->thisline->StationList[t].Kms - head->thisline->StationList[head->start].Kms;
					if (kms < 0)kms *= -1;
					int primekm = head->prime->thisline->StationList[head->prime->start].Kms - head->prime->thisline->StationList[head->prime->end].Kms;
					if (primekm < 0)primekm *= -1;
					if (kms == primekm) {
						if ((linemode == 3 && head->thisline->Mode) || (linemode == head->thisline->Mode) || (head->thisline->Mode == head->prime->thisline->Mode) || head->thisline->Mode == 3 || H12_StationLine(head->thisline->StationList[t].Station, head->thisline->StationList[head->start].Station, head->thisline - LINE)) {
							head->start = t;
							way* n = head->prime;
							head->prime = head->prime->prime;
							if (head->prime != NULL) {
								head->prime->next = head;
							}
							else start = head;
							free(n);
							continue;
						}
					}
				}
			}
		}
		head = head->prime;
	}
	return;
}
int H19_LinkListLength(way* head) {
	int length = 0;
	while (1) {
		if (head == NULL)return length;
		head = head->next;
		length++;
	}
}
void Func1_FileMaker() {
	H2_Initilize();
	static int set = 0;
	if (set == 0) {
		printf("请输入获取文件的密码，按回车结束：");
		char password[40] = { 0 };
		int length = 0;
		while (1) {
			char t = _getch();
			if (t < 0)continue;
			if (t == '\r' || t == '\n') {
				putchar('\n');
				break;
			}
			if (t == '\b') {
				if (length == 0)continue;
				putchar('\b');
				putchar(' ');
				putchar('\b');
				length--;
				password[length] = 0;
			}
			else {
				if (length == 39)continue;
				password[length] = t;
				putchar('*');
				length++;
			}
		}
		if (strcmp(password, "DataOpen") != 0)return (void)MessageBoxA(NULL, "密码错误！", "错误提示", MB_OK | MB_SETFOREGROUND | MB_ICONERROR);
		set = 1;
		printf("Success:权限验证成功\n");
	}
	else printf("Success:已获得权限\n");
	FILE* fp = NULL;
	OPENFILENAME ofn;
	char path[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(0, path, CSIDL_DESKTOPDIRECTORY, 0);
	char pathf[MAX_PATH] = { 0 };
	sprintf_s(pathf, sizeof(pathf), "%s\\中国铁路客运里程表 ", path);
	TIME t("本地");
	t.show(pathf + strlen(pathf), sizeof(pathf) - strlen(pathf), "date 时间");
	sprintf_s(pathf + strlen(pathf), sizeof(pathf) - strlen(pathf), ".txt");
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn); // 结构大小
	ofn.lpstrFile = pathf; // 路径
	ofn.nMaxFile = MAX_PATH; // 路径大小
	ofn.lpstrInitialDir = path;
	ofn.lpstrTitle = "选择保存路径";
	ofn.lpstrFilter = "文本文件(*.txt)\0*.txt\0\0"; // 文件类型
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	printf("Info:正在准备文件选择器\n");
	if (GetSaveFileName(&ofn) == 0)return (void)MessageBoxA(NULL, "选择文件无效", "错误提示", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
	printf("Success:已选择文件路径：%s\n", pathf);
	if (_access(pathf, 0) == 0) {
		if (MessageBoxA(NULL, "您选择的文件已存在，请问是否要覆盖这个文件？", "提示", MB_YESNO | MB_ICONINFORMATION | MB_SETFOREGROUND) == IDNO) {
			Func1_FileMaker();
			return;
		}
	}
	fopen_s(&fp, pathf, "w");
	if (fp == NULL)return (void)MessageBoxA(NULL, "文件写入失败", "错误提示", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
	printf("Success:文件输入流打开成功\n");
	for (int i = 0; i < LINE_SIZE; i++) {
		line t = LINE[i];
		fprintf(fp, "%s\n", t.Name);
		for (int j = 0; j < t.StationNumbers; j++) {
			station st = STATION[t.StationList[j].Station - 1];
			fprintf(fp, "%d\t%s\t%s\n", t.StationList[j].Kms, PROVINCE[st.Province - 1].Name, st.Name);
		}
		fputc('\n', fp);
	}
	fclose(fp);
	printf("Success:文件写入成功\n");
	printf("Success:文件输入流已关闭\n");
	if (MessageBoxA(NULL, "文件写入成功，是否立即打开文件？", "提示", MB_YESNO | MB_ICONINFORMATION | MB_SETFOREGROUND) == IDYES) {
		char temp[200] = { 0 };
		sprintf_s(temp, sizeof(temp), "\"%s\"", pathf);
		printf("Success:已为您打开文件\n");
		std::thread t(system, std::ref(temp));
		t.detach();
	}
	else printf("Fail:您拒绝打开文件，但是文件仍然保留\n");
	Sleep(2000);
	return;
}
int Func2_StationGet(int mode,int starty=0) {
	COORD coord = H1_CursorPosition();
	H6_gotoxy(0, starty);
	switch (mode) {
	case 0:printf("检索信息为车站名或拼音首字母，按\"↑\"、\"↓\"和回车进行选择。\n请输入车站检索信息进行检索："); break;
	case 1:printf("检索信息为车站名或拼音首字母，按\"↑\"、\"↓\"和回车进行选择。\n请输入发站检索信息进行检索："); break;
	case 2:printf("检索信息为车站名或拼音首字母，按\"↑\"、\"↓\"和回车进行选择。\n请输入到站检索信息进行检索："); break;
	}
	coord = H1_CursorPosition();
	H7_StationList(NULL, 0, 0, coord, -1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("            ");
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	char t[16] = { 0 }, temp1 = 0, temp2 = 0;
	int list[400] = { 0 }, count = 0;
	while (1) {
		if ((temp1 = _getch()) < 0) {
			temp2 = _getch();
			if (temp1 == -32) {
				if (strlen(t) < 2)continue;
				else {
					H4_StationSearch(t, list, 400);
					for (int i = 0; i < 400; i++)if (list[i] == 0) {
						count = i;
						break;
					}
					if (temp2 == 72)H7_StationList(list, count, -1, coord,-1);
					if (temp2 == 80)H7_StationList(list, count, 1, coord,-1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				}
				continue;
			}
			if (t[0] > 0)continue;
			else {
				if (strlen(t) >= 12)continue;
				t[strlen(t)] = temp1;
				t[strlen(t)] = temp2;
				putchar(temp1);
				putchar(temp2);
				for (int i = 0; i < 400; i++)list[i] = 0;
				H4_StationSearch(t, list, 400);
				for (int i = 0; i < 400; i++)if (list[i] == 0) {
					count = i;
					break;
				}
				coord.X += 2;
				H7_StationList(NULL, 0, 0, coord,-1);
				H7_StationList(list, count, 3, coord,-1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			}
		}
		else {
			if (temp1 == 10 || temp1 == 13) {
				if (strlen(t) > 1) {
					if (count == 0)continue;
					int t = H7_StationList(list, count, 2, coord,-1);
					H7_StationList(list, count, -2, coord,-1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					return t;
				}
				else continue;
			}
			if (isalpha(temp1)) {
				if (strlen(t) >= 12)continue;
				if (t[0] < 0)continue;
				if (temp1 >= 'a' && temp1 <= 'z')temp1 += 'A' - 'a';
				int r = strlen(t);
				t[r] = temp1;
				t[r+1] = 0;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				putchar(temp1);
				coord.X++;
				if (r >= 1) {
					for (int i = 0; i < 400; i++)list[i] = 0;
					H4_StationSearch(t, list, 400);
					for (int i = 0; i < 400; i++)if (list[i] == 0) {
						count = i;
						break;
					}
					H7_StationList(NULL, 0, 0, coord,-1);
					H7_StationList(list, count, 3, coord,-1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				}
			}
			else if (temp1 == '\b') {
				int r = strlen(t);
				if (r == 0)continue;
				if (t[0] > 0) {
					coord.X--;
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					putchar(' ');
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					t[r - 1] = 0;
					if (r >= 3) {
						for (int i = 0; i < 400; i++)list[i] = 0;
						H4_StationSearch(t, list, 400);
						for (int i = 0; i < 400; i++)if (list[i] == 0) {
							count = i;
							break;
						}
						H7_StationList(list, count, 3, coord,-1);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					}
					if (r <= 2) {
						H7_StationList(NULL, 0, 0, coord,-1);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					}
					continue;
				}
				else if (t[0] < 0) {
					coord.X -= 2;
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					printf("  ");
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					t[r - 2] = t[r - 1] = 0;
					if (r >= 4) {
						for (int i = 0; i < 400; i++)list[i] = 0;
						H4_StationSearch(t, list, 400);
						for (int i = 0; i < 400; i++)if (list[i] == 0) {
							count = i;
							break;
						}
						H7_StationList(list, count, 3, coord,-1);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					}
					if (r <= 2) {
						H7_StationList(NULL, 0, 0, coord,-1);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					}
				}
			}
		}
	}
}
int Func3_StationForSE(int* t1, int* t2, int avd[]) {
	H2_Initilize();
	int st = Func2_StationGet(1), et = Func2_StationGet(2);
	station start = STATION[st - 1], end = STATION[et - 1];
	int sl1 = strlen(start.Name) > strlen(end.Name) ? strlen(start.Name) : strlen(end.Name);
	line sl = LINE[H5_StationLocate(st, NULL, 0, 1)], el = LINE[H5_StationLocate(et, NULL, 0, 1)];
	system("cls");
	printf("已选定出发站为：");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 63);
	for (int i = printf("%s", start.Name); i < sl1; i++)putchar(' ');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32 | BACKGROUND_INTENSITY);
	for (int i = printf("（%s，%s%s）", PROVINCE[start.Province - 1].Name, sl.Name, start.Freq > 1 ? "等" : ""); i < 20; i++)putchar(' ');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("；\n已选定到达站为：");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 63);
	for (int i = printf("%s", end.Name); i < sl1; i++)putchar(' ');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32 | BACKGROUND_INTENSITY);
	for (int i = printf("（%s，%s%s）", PROVINCE[end.Province - 1].Name, el.Name, end.Freq > 1 ? "等" : ""); i < 20; i++)putchar(' ');
	if (st == et) {
		*t1 = st;
		*t2 = et;
		return 0;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("。\n按1开始选择径路计算\n按2直接自动计算最短路径\n按3在设置需要绕开的车站后计算最短路径\n按\"Esc\"返回。");
	while (1) {
		char t = _getch();
		if (t == '1') {
			*t1 = st;
			*t2 = et;
			return 0;
		}
		if (t == '2') {
			*t1 = st;
			*t2 = et;
			return 1;
		}
		if (t == '3') {
			*t1 = st;
			*t2 = et;
			H2_Initilize();
			int leng = 0;
			printf("请按键选择要避开的车站数量（1-9）");
			while (1) {
				char ch = _getch();
				if (ch >= '1' && ch <= '9')leng = ch - '0';
				else continue;
				break;
			}
			for (int i = 0; i <= leng; i++)avd[i] = 0;
			system("cls");
			for (int i = 0; i < leng; i++) {
				if (i) {
					H6_gotoxy(0, 1);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("已选中车站：");
					for (int j = 0; j < i; j++) {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 64);
						printf("%s", STATION[avd[j] - 1].Name);
						if (strcmp(STATION[avd[j] - 1].Name - 2 + strlen(STATION[avd[j] - 1].Name), "所") != 0)printf("站");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
						if (j != i - 1)printf("、");
					}
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				H6_gotoxy(0, 0);
				printf("选择避开的车站（%d/%d，允许重复选择）：", i + 1, leng);
				int m = Func2_StationGet(0,2);
				if (m == *t1 || m == *t2) {
					i--;
					MessageBoxA(NULL, "选择的车站不能与出发站、到达站相同！", "错误提示", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
					continue;
				}
				avd[i] = m;
			}
			return 1;
		}
		if (t == 27) {
			*t1 = *t2 = -1;
			return 0;
		}
	}
}
int Func4_LineGet() {
	H2_Initilize();
	printf("检索信息为线路名或拼音首字母，按\"↑\"、\"↓\"和回车进行选择。\n请输入线路检索信息进行检索：");
	COORD coord = H1_CursorPosition();
	H8_LineList(NULL, 0, 0, coord);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("            ");
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	char t[24] = { 0 }, temp1 = 0, temp2 = 0;
	int list[40] = { 0 }, count = 0;
	while (1) {
		if ((temp1 = _getch()) < 0) {
			temp2 = _getch();
			if (temp1 == -32) {
				if (strlen(t) < 2)continue;
				else {
					for (int i = 0; i < 40; i++)list[i] = -1;
					H10_LineSearch(t, list, 40);
					for (int i = 1; i < 40; i++)if (list[i] == -1) {
						count = i;
						break;
					}
					if (temp2 == 72)H8_LineList(list, count, -1, coord);
					if (temp2 == 80)H8_LineList(list, count, 1, coord);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				}
				continue;
			}
			if (t[0] > 0)continue;
			else {
				if (strlen(t) >= 12)continue;
				t[strlen(t)] = temp1;
				t[strlen(t)] = temp2;
				putchar(temp1);
				putchar(temp2);
				for (int i = 0; i < 40; i++)list[i] = -1;
				H10_LineSearch(t, list, 40);
				for (int i = 0; i < 40; i++)if (list[i] == -1) {
					count = i;
					break;
				}
				coord.X += 2;
				H8_LineList(NULL, 0, 0, coord);
				H8_LineList(list, count, 3, coord);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			}
		}
		else {
			if (temp1 == 10 || temp1 == 13) {
				if (strlen(t) > 1) {
					if (count == 0)continue;
					int t = H8_LineList(list, count, 2, coord);
					H8_LineList(list, count, -2, coord);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					return t;
				}
				else continue;
			}
			if (isalpha(temp1)) {
				if (strlen(t) >= 12)continue;
				if (t[0] < 0)continue;
				if (temp1 >= 'a' && temp1 <= 'z')temp1 += 'A' - 'a';
				int r = strlen(t);
				t[r] = temp1;
				t[r + 1] = 0;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				putchar(temp1);
				coord.X++;
				if (r >= 1) {
					for (int i = 0; i < 40; i++)list[i] = -1;
					H10_LineSearch(t, list, 40);
					for (int i = 0; i < 40; i++)if (list[i] == -1) {
						count = i;
						break;
					}
					H8_LineList(NULL, 0, 0, coord);
					H8_LineList(list, count, 3, coord);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				}
			}
			else if (temp1 == '\b') {
				int r = strlen(t);
				if (r == 0)continue;
				if (t[0] > 0) {
					coord.X--;
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					putchar(' ');
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					t[r - 1] = 0;
					if (r >= 3) {
						for (int i = 0; i < 40; i++)list[i] = -1;
						H10_LineSearch(t, list, 40);
						for (int i = 0; i < 40; i++)if (list[i] == -1) {
							count = i;
							break;
						}
						H8_LineList(list, count, 3, coord);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					}
					if (r <= 2) {
						H8_LineList(NULL, 0, 0, coord);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					}
					continue;
				}
				else if (t[0] < 0) {
					coord.X -= 2;
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					printf("  ");
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					t[r - 2] = t[r - 1] = 0;
					if (r >= 4) {
						for (int i = 0; i < 40; i++)list[i] = -1;
						H10_LineSearch(t, list, 40);
						for (int i = 0; i < 40; i++)if (list[i] == -1) {
							count = i;
							break;
						}
						H8_LineList(list, count, 3, coord);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					}
					if (r <= 2) {
						H8_LineList(NULL, 0, 0, coord);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					}
				}
			}
		}
	}
}
void Func5_LineShow() {
	line t = LINE[Func4_LineGet()];
	int start = 0;
	while (1) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		system("cls");
		printf("当前显示线路");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
		printf("%s", t.Name);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("的里程信息。按Esc返回");
		if (t.StationNumbers > 45)printf("，按\"↑\"、\"↓\"键翻页。");
		else printf("。");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 15; j++) {
				if (45 * start + i * 15 + j >= t.StationNumbers)break;
				station st = STATION[t.StationList[45 * start + 15 * i + j].Station - 1];
				char m[40] = { 0 };
				if (st.Freq > 1) {
					int s[2] = { 0 };
					H5_StationLocate(t.StationList[45 * start + 15 * i + j].Station, s, 2, 0);
					if (LINE[s[0]].StationList == t.StationList)s[0] = s[1];
					sprintf_s(m, sizeof(m), "%s（%s%s）", st.Name, LINE[s[0]].Name, st.Freq > 2 ? "等" : "");
				}
				else sprintf_s(m, sizeof(m), "%s", st.Name);
				H6_gotoxy(40 * i, 1 + 2 * j);
				printf("%-4d %s %-20s", t.StationList[45 * start + 15 * i + j].Kms, PROVINCE[st.Province - 1].Name, m);
			}
		}
		while (1) {
			char ct = _getch();
			if (ct == 27)return;
			if (ct == -32) {
				ct = _getch();
				if (ct == 72) {
					if (start == 0)continue;
					start--;
					break;
				}
				if (ct == 80) {
					if (start == ((t.StationNumbers - 1) / 45))continue;
					start++;
					break;
				}
			}
		}
	}
	
}
void Func6_ShortLength(int t1, int t2, int avd[]) {
	if (t1 == t2)return (void)MessageBoxA(NULL, "发站与到站不能相同！", "提示", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
	H2_Initilize();
	int avdl = 0;
	while (1) {
		if (avd[avdl] != 0)avdl++;
		else break;
	}
	int Mode_Start = H14_StationMode(t1), Mode_End = H14_StationMode(t2);
	int ModeLine = 0, setmode = 0;
	if (Mode_Start == 0 || Mode_End == 0) {
		MessageBoxA(NULL, "选择车站无法计算，问题：\n    涉及不运行客车的区间。", "错误提示", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		return;
	}
	if ((STATION[t1 - 1].Province == 22 && STATION[t2 - 1].Province != 22) || (STATION[t1 - 1].Province != 22 && STATION[t2 - 1].Province == 22)) {
		if(Mode_Start == 2 || Mode_End == 2)ModeLine = 3;
		else {
			printf("请按键选择计算模式：1.普车；3.混合。");
			while (1) {
				char t = _getch();
				if (t == '1') {
					ModeLine = 1;
					break;
				}
				if (t == '3') {
					ModeLine = 3;
					break;
				}
			}
		}
	}
	else {
		if ((Mode_Start == 1 || Mode_Start == 3) && (Mode_End == 1 || Mode_End == 3))ModeLine += 1;
		if ((Mode_Start == 2 || Mode_Start == 3) && (Mode_End == 2 || Mode_End == 3))ModeLine += 2;
		if (ModeLine == 0)ModeLine = 3;
		else {
			if (ModeLine == 1) {
				printf("请按键选择计算模式：1.普车；3.混合。");
				while (1) {
					char t = _getch();
					if (t == '1') {
						ModeLine = 1;
						break;
					}
					if (t == '3') {
						ModeLine = 3;
						break;
					}
				}
			}
			else if (ModeLine == 2) {
				printf("请按键选择计算模式：2.动车；3.混合。");
				while (1) {
					char t = _getch();
					if (t == '2') {
						ModeLine = 2;
						break;
					}
					if (t == '3') {
						ModeLine = 3;
						break;
					}
				}
			}
			else if (ModeLine == 3) {
				printf("请按键选择计算模式：1.普车；2.动车；3.混合。");
				while (1) {
					char t = _getch();
					if (t == '1') {
						ModeLine = 1;
						break;
					}
					if (t == '2') {
						ModeLine = 2;
						break;
					}
					if (t == '3') {
						ModeLine = 3;
						break;
					}
				}
			}
		}
	}
	short int EX[1200] = { 0 }, EXcount = 0, start_locate = 0, end_locate = 0;
	for (int i = 0; i < STATION_SIZE; i++)if (STATION[i].Freq > 1)EX[EXcount++] = i + 1;
	if (STATION[t1 - 1].Freq <= 1)EX[EXcount++] = t1;
	if (STATION[t2 - 1].Freq <= 1) {
		EX[EXcount++] = t2;
		STATION[t2 - 1].Freq = 2;
		setmode = 1;
	}
	for (int i = 0; i < EXcount; i++) {
		if (EX[i] == t1)start_locate = i;
		if (EX[i] == t2)end_locate = i;
	}
	short int* REX = (short int*)malloc(STATION_SIZE * sizeof(short int));
	step* p = (step*)malloc(200 * sizeof(step) * EXcount);
	if (p == NULL || REX == NULL)return (void)MessageBoxA(NULL, "内存不足！", "提示", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
	for (int i = 0; i < STATION_SIZE; i++)REX[i] = 0;
	/*反查询*/
	for (int i = 0; i < EXcount; i++)REX[EX[i] - 1] = i;
	step* Step[200] = { 0 };
	for (int i = 0; i < 200; i++)Step[i] = (step*)(p + (long long)EXcount * i);
	for (int i = 200 * EXcount - 1; i >= 0; i--)p[i] = { 0,-1,32767 };
	int line[20] = { 0 }, linecount = 0;
	for (int i = 0; i < 20; i++)line[i] = -1;
	H5_StationLocate(t1, line, 20, 0);
	for (int i = 0; i < 20; i++) {
		if (line[i] == -1) {
			linecount = i;
			break;
		}
	}
	//FILE* fp = NULL;
	//fp = fopen("test.txt", "w");
	//fp = stdout;
	Step[0][REX[t1 - 1]] = { (short)start_locate,(short)line[0],(short)0 };
	for (int i = 0; i < linecount; i++) {
		int km = 0, locate = H17_LocateStationInLine(line[i], t1);
		int last = H15_LastExStation(line[i], t1, ModeLine, avd, avdl), next = H16_NextExStation(line[i], t1, ModeLine, avd, avdl);
		if (last != -1) {
			km = LINE[line[i]].StationList[locate].Kms - LINE[line[i]].StationList[last].Kms;
			if (km < Step[0][REX[LINE[line[i]].StationList[last].Station - 1]].Kms)Step[0][REX[LINE[line[i]].StationList[last].Station - 1]] = { (short)start_locate,(short)line[i],(short)km };
		}
		if (next != -1) {
			km = LINE[line[i]].StationList[next].Kms - LINE[line[i]].StationList[locate].Kms;
			if (km < Step[0][REX[LINE[line[i]].StationList[next].Station - 1]].Kms)Step[0][REX[LINE[line[i]].StationList[next].Station - 1]] = { (short)start_locate,(short)line[i],(short)km };
		}
	}
	way* head = NULL;
	int least_kms = 32767;
	for (int r = 0; r <= 200; r++) {
		//fprintf(fp,"\n\n操作第%d次：",r);
		//(void)getchar();
		if (r == 199) {
			MessageBoxA(NULL, "计算失败，可能原因：发站与到站在已设置的条件下不能连通。", "提示", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			free(p);
			free(REX);
			return;
		}
		if (Step[r][end_locate].Kms != 32767&&r) {
			int length = Step[r][end_locate].Kms;
			int swi = 0;
			for (int j = 0; j < EXcount; j++) {
				if (Step[r][j].Kms <= length) {
					if (Step[r][j].Kms < Step[r - 1][j].Kms) {
						swi = 1;
						break;
					}
				}
			}
			if (swi == 0) {
				/*计算结束*/
				least_kms = Step[r][end_locate].Kms;
				int t = r, now_locate = end_locate;
				while (1) {
					way* wp = (way*)malloc(sizeof(way));
					if (wp == NULL)return (void)MessageBoxA(NULL, "计算完毕但内存不足！", "错误", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
					wp->next = head;
					if (head != NULL)head->prime = wp;
					head = wp;
					head->thisline = &LINE[Step[t][now_locate].line];
					head->end = H17_LocateStationInLine(Step[t][now_locate].line, EX[now_locate]);
					head->start = H17_LocateStationInLine(Step[t][now_locate].line, EX[Step[t][now_locate].last]);
					now_locate = Step[t][now_locate].last;
					t--;
					if (t == -1)break;
				}
				head->prime = NULL;
				break;
			}
		}
		for (int i = 0; i < EXcount; i++)Step[r + 1][i] = { (short)i,Step[r][i].line,Step[r][i].Kms };
		for (int j = 0; j < EXcount; j++) {
			if ((r == 0 && Step[r][j].Kms != 32767) || (r >= 1 && Step[r][j].Kms != Step[r - 1][j].Kms)) {
				//fprintf(fp, "\n车站：%s 里程：%d\n", STATION[EX[j] - 1].Name, Step[r][j].Kms);
				int station = EX[j];
				for (int i = 0; i < 20; i++)line[i] = -1;
				H5_StationLocate(station, line, 20, 0);
				for (int i = 0; i < 20; i++) {
					if (line[i] == -1) {
						linecount = i;
						break;
					}
				}
				for (int i = 0; i < linecount; i++) {
					//fprintf(fp, "操作线路：%s\t", LINE[line[i]].Name);
					int km = 0, locate = H17_LocateStationInLine(line[i], station);
					int last = H15_LastExStation(line[i], station, ModeLine, avd, avdl), next = H16_NextExStation(line[i], station, ModeLine, avd, avdl);
					if (last != -1) {
						//fprintf(fp, "被操作站：%s\t", STATION[LINE[line[i]].StationList[last].Station - 1].Name);
						km = LINE[line[i]].StationList[locate].Kms - LINE[line[i]].StationList[last].Kms;
						int lastindex = REX[LINE[line[i]].StationList[last].Station - 1];
						if (Step[r + 1][lastindex].Kms > Step[r][j].Kms + km)Step[r + 1][lastindex] = { (short)j,(short)line[i],(short)(Step[r][j].Kms + km) };
						else if (Step[r + 1][lastindex].Kms == Step[r][j].Kms + km) {
							if (line[i] == Step[r][j].line) {
								if (ModeLine == 3 || LINE[line[i]].Mode == 3 || LINE[line[i]].Mode == ModeLine || (H12_StationLine(EX[j], EX[lastindex], line[i]) == 1 && LINE[line[i]].Mode_Set_Information && ((LINE[line[i]].Mode_Set_Information[0] - '0') == ModeLine)))Step[r + 1][lastindex] = { (short)j,(short)line[i],(short)(Step[r][j].Kms + km) };
							}
						}
					}
					if (next != -1) {
						//fprintf(fp, "被操作站：%s\t", STATION[LINE[line[i]].StationList[next].Station - 1].Name);
						km = LINE[line[i]].StationList[next].Kms - LINE[line[i]].StationList[locate].Kms;
						int nextindex = REX[LINE[line[i]].StationList[next].Station - 1];
						if (Step[r + 1][nextindex].Kms > Step[r][j].Kms + km)Step[r + 1][nextindex] = { (short)j,(short)line[i],(short)(Step[r][j].Kms + km) };
						else if (Step[r + 1][nextindex].Kms == Step[r][j].Kms + km) {
							if (line[i] == Step[r][j].line) {
								if (ModeLine == 3 || LINE[line[i]].Mode == 3 || LINE[line[i]].Mode == ModeLine || (H12_StationLine(EX[j], EX[nextindex], line[i]) == 1 && LINE[line[i]].Mode_Set_Information && ((LINE[line[i]].Mode_Set_Information[0] - '0') == ModeLine)))Step[r + 1][nextindex] = { (short)j,(short)line[i],(short)(Step[r][j].Kms + km) };
							}
						}
					}
				}
			}
		}
	}
	if (setmode == 1)STATION[t2 - 1].Freq = 1;
	/*别忘了free*/
	free(p);
	free(REX);
	fflush(stdout);
	fflush(stdin);
	//fclose(fp);
	int error = 0;
	H18_WaySort(head, 1, ModeLine, error, avd, avdl);
	int listlength = H19_LinkListLength(head);
	if (listlength >= 27 && listlength < 37)system("mode con cols=120 lines=40");
	if (listlength >= 37 && listlength < 47)system("mode con cols=120 lines=50");
	if (listlength >= 47 && listlength < 57)system("mode con cols=120 lines=60");
	if (t1 == t2) {
		least_kms = 0;
		H11_DestroyLinkList(head);
		head = NULL;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	system("cls");
	printf("由");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79);
	if (strcmp(STATION[t1 - 1].Name + strlen(STATION[t1 - 1].Name) - 2, "所") == 0)printf("%s", STATION[t1 - 1].Name);
	else printf("%s站", STATION[t1 - 1].Name);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("至");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79);
	if (strcmp(STATION[t2 - 1].Name + strlen(STATION[t2 - 1].Name) - 2, "所")==0)printf("%s", STATION[t2 - 1].Name);
	else printf("%s站", STATION[t2 - 1].Name);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("在");
	if (avdl != 0) {
		printf("避开");
		for (int j = 0; j < avdl; j++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 64);
			printf("%s", STATION[avd[j] - 1].Name);
			if (strcmp(STATION[avd[j] - 1].Name - 2 + strlen(STATION[avd[j] - 1].Name), "所") != 0)printf("站");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			if (j != avdl - 1)printf("、");
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("的");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 95);
	switch (ModeLine) {
	case 1:printf("普车"); break;
	case 2:printf("动车"); break;
	case 3:printf("混合"); break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("模式下的最短里程为：");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31);
	printf("%dkm", least_kms);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("，路径如下：\n");
	H9_LinkPrint(head, 32767);
	const char* errp_HengYang = "请注意：衡阳枢纽涉及吉衡线、怀衡线转京广线的列车，实际运行时上下行线路、里程均不同，"
		"吉安、怀化经衡阳枢纽往广州方向实际经：\n\t吉衡线/怀衡线-长塘所-茶山坳联络线-茶山坳-京广线\n运行，不经衡阳北站。";
	const char* errp_XiNing = "请注意：兰新客专线兰州西至西宁段停运，动车组经兰青线运行，国铁集团计算里程时，动车组列车"
		"在兰青线的：\n\t海石湾站\t乐都站\t平安驿站\t等站\n里程按兰新客专：\n\t民和南站\t海东站\t海东西站\t等站\n计算。"
		"但是本程序仍然按照兰青线的里程计算。";
	if (error & HengYang)MessageBoxA(NULL,errp_HengYang , "提示", MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
	if (error & XiNing)MessageBoxA(NULL,errp_XiNing , "提示", MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("计算和提示进行完毕，按Esc返回。");
	while (1) {
		if (_getch() == 27) {
			H11_DestroyLinkList(head);
			if (listlength >= 27)H2_Initilize();
			else system("cls");
			return;
		}
	}
	//fclose(fp);
}
void Func7_LineLength() {
	H2_Initilize();
	int t1 = 0, t2 = 0, avd[10] = { 0 };
	if (Func3_StationForSE(&t1, &t2, avd) == 1)return Func6_ShortLength(t1, t2, avd);
	if (t1 == -1) {
		system("cls");
		printf("已停止计算！");
		return;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	system("cls");
	printf("出发车站：%s\n", STATION[t1 - 1].Name);
	COORD cstart = H1_CursorPosition();
	way* head = NULL, * end = NULL;
	int start = t1, _end = 0, linen = 0, lines[20] = { 0 };
	char temp1 = 0, temp2 = 0;
	while (1) {
		H2_Initilize();
		H9_LinkPrint(head, 6);
		for (int i = 0; i < 20; i++)lines[i] = 0;
		station now = STATION[start - 1];
		H5_StationLocate(start, lines, 20, 0);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("当前选择在车站“%s”（%s）的线路。", now.Name, PROVINCE[now.Province - 1].Name);
		printf("请按\"↑\"、\"↓\"和回车进行选择。");
		COORD coord = H1_CursorPosition();
		H8_LineList(NULL, 0, 0, coord);
		H8_LineList(lines, now.Freq, -1, coord);
		while (1) {
			temp1 = _getch();
			if (temp1 == 10 || temp1 == 13) {
				linen = H8_LineList(lines, 0, 2, coord);
				H8_LineList(NULL, 0, -2, coord);
				break;
			}
			if (temp1 == -32) {
				temp2 = _getch();
				if (temp2 == 72)H8_LineList(lines, now.Freq, -1, coord);
				if (temp2 == 80)H8_LineList(lines, now.Freq, 1, coord);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			}
		}
		H8_LineList(NULL, 0, -2, coord);
		line linet = LINE[linen];
		coord.X = 0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("已选择线路：%s（%s—%s）                                    ", linet.Name, STATION[linet.StationList[0].Station - 1].Name, STATION[linet.StationList[linet.StationNumbers - 1].Station - 1].Name);
		way* p = (way*)malloc(sizeof(way));
		if (p == NULL)exit(3);
		if (head == NULL || end == NULL) {
			head = end = p;
			p->prime = NULL;
			p->next = NULL;
		}
		else {
			end->next = p;
			p->prime = end;
			end = p;
		}
		p->next = NULL;
		p->thisline = &LINE[linen];
		int Stationlist[50] = { 0 }, count = 0;
		const stationlist* lt = linet.StationList;
		for (int i = 0; i < linet.StationNumbers; i++) {
			if (start == lt[i].Station)p->start = i;
			if ((STATION[lt[i].Station - 1].Freq > 1 || t2 == lt[i].Station) && lt[i].Station != start)Stationlist[count++] = lt[i].Station;
		}
		if (count == 0) {
			MessageBoxA(NULL, "已走入末端线路，点击确定退出！", "错误提示", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			if (head != NULL)H11_DestroyLinkList(head);
			return;
		}
		printf("\n选择当前线路的车站：");
		coord = H1_CursorPosition();
		H7_StationList(NULL, 0, 0, coord, -1);
		H7_StationList(Stationlist, count, -1, coord, linen);
		while (1) {
			temp1 = _getch();
			if (temp1 == 10 || temp1 == 13) {
				_end = H7_StationList(Stationlist, count, 2, coord, linen);
				H7_StationList(Stationlist, count, -2, coord, linen);
				break;
			}
			if (temp1 == -32) {
				temp2 = _getch();
				if (temp2 == 72)H7_StationList(Stationlist, count, -1, coord, linen);
				if (temp2 == 80)H7_StationList(Stationlist, count, 1, coord, linen);
			}
		}

		for (int i = 0; i < linet.StationNumbers; i++)if (_end == linet.StationList[i].Station)p->end = i;
		coord.X = 0;
		H7_StationList(NULL, 0, -2, coord, -1);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("已选择车站：%s                    \n", STATION[_end - 1].Name);
		if (_end == t2)break;
		start = _end;
	}
	int listlength = H19_LinkListLength(head);
	if (listlength >= 27 && listlength < 37)system("mode con cols=120 lines=40");
	if (listlength >= 37 && listlength < 47)system("mode con cols=120 lines=50");
	if (listlength >= 47 && listlength < 57)system("mode con cols=120 lines=60");
	if (listlength >= 57 && listlength < 67)system("mode con cols=120 lines=70");
	if (listlength >= 67)system("mode con cols=120 lines=100");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	system("cls");
	printf("您选择的由");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79);
	printf("%s站", STATION[t1 - 1].Name);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("至");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79);
	printf("%s站", STATION[t2 - 1].Name);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("的线路为：\n");
	int l = H9_LinkPrint(head, 32767);
	H11_DestroyLinkList(head);
	printf("总里程为：");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 95 | BACKGROUND_INTENSITY);
	printf("%d千米", l);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("。\n计算完毕，按Esc返回。");
	while (1) {
		if (_getch() == 27) {
			if (listlength >= 27)H2_Initilize();
			else system("cls");
			return;
		}
	}
}
void Func8_HelpInfo() {
	const char* p = "使用说明：\n"
		"   本程序根据网络查找得到的2021年年初的中国铁路客运运价里程表计算，包括中"
		"国大陆在文件中的线路、部分2021年至2022年初已开通或计划开通的铁路（数据为"
		"作者自行查询计算得到）、广深港高铁香港段，暂时不包括台湾铁路。里程表中的部"
		"分数据经过作者个人改动，主要目的为方便计算并修改原文件中的奇怪问题。包括但"
		"不限于：\n"
		"    ▲合并线路（如京广高速线京郑段、郑武段、武广段合并为京广高速线，宁蓉线"
		"宁渝段、 成渝段合并为宁蓉线等）；\n"
		"    ▲拆分线路（如宝成、成渝线拆分为宝成线和成渝线等）；\n"
		"    ▲修改线路（如宁启线、哈环线等线路由于结构导致程序难以计算，因此将原宁启"
		"线（林场、永宁镇—启东）拆分为宁启线（林场—启东）和京沪宁启联络"
		"线（永宁镇—六合），原哈环线（哈尔滨—哈尔滨）拆分为哈环线（哈尔滨—香"
		"坊）的优弧，香坊至哈尔滨的劣弧可用滨绥线替代。\n"
		"   在程序的“自动计算最短路径”中，普车模式考虑绝大多数普速列车（包括常见的普通车、动力集中型动"
		"车组列车、北京市郊铁路列车）所涉及的线路，动车模式考虑动力分"
		"散型动车组（包括长株潭城际等普速动车，不包括仅运行动力集中型动车组列车的线路）能够到达的线路。"
		"2021年中调图取消了大量既有线动力分散型动车组列车，相关线路（如京哈线京秦段）已经不在动车模式范围内。"
		"可选的模式由程序根据选择的车站分析给出。"
		"线路能跑什么车，根据作者根据个人了解所设定。因此选择"
		"普速或动车模式，可能存在问题，希望用户发现问题后能联系作者。本功能同时也考虑到了某些线路不运行客"
		"车的问题，在影响计算结果的情况下，最短路径计算中不考虑这些线路，如津山线狼窝铺至龙家营区间比京哈"
		"线短，但不运行客车，因此不再考虑。\n"
		"   在自动计算经过某些路段时，会根据情况给出提示。\n"
		"   在“自选线路计算里程”中，选定线路之后，只显示线路上的可跨线车站和终到站。请勿走入死胡同。\n"
		"   本程序的使用界面应该相当友好，跟着提示操作就行。\n"
		"   另外，本程序仅为相关爱好者所做，请勿作为实际乘车、购票的标准使用。请勿传"
		"播。如有问题，可联系作者邮箱：2434727792@qq.com反映。\n"
		"版本：2.0 2021/10/30";
	MessageBoxA(NULL, std::ref(p), "使用说明", MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
	return;
}
void Func9_LineByStation() {
	system("cls");
	int Station = Func2_StationGet(0);
	int Line[20] = { 0 }, Linecount = 0, show = 0;
	for (int i = 0; i < 20; i++)Line[i] = -1;
	H5_StationLocate(Station, Line, 20, 0);
	for (int i = 0; i < 20; i++)if (Line[i] == -1) {
		Linecount = i;
		break;
	}
	char temp1 = 0, temp2 = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	system("cls");
	printf("按\"↑\"、\"↓\"和回车进行选择线路。");
	COORD coord = H1_CursorPosition();
	H8_LineList(NULL, 0, 0, coord);
	H8_LineList(Line, Linecount, 3, coord);
	while (1) {
		if ((temp1 = _getch()) < 0) {
			temp2 = _getch();
			if (temp2 == 72)H8_LineList(Line, Linecount, -1, coord);
			if (temp2 == 80)H8_LineList(Line, Linecount, 1, coord);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31 | BACKGROUND_INTENSITY);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			continue;
		}
		else {
			if (temp1 == 10 || temp1 == 13) {
				show = H8_LineList(Line, Linecount, 2, coord);
				H8_LineList(Line, Linecount, -2, coord);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				break;
			}
			else continue;
		}
	}

	line t = LINE[show];
	int start = 0;
	while (1) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		system("cls");
		printf("当前显示线路");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
		printf("%s", t.Name);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("的里程信息。按Esc返回");
		if (t.StationNumbers > 45)printf("，按\"↑\"、\"↓\"键翻页。");
		else printf("。");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 15; j++) {
				if (45 * start + i * 15 + j >= t.StationNumbers)break;
				station st = STATION[t.StationList[45 * start + 15 * i + j].Station - 1];
				char m[32] = { 0 };
				if (st.Freq > 1) {
					int s[2] = { 0 };
					H5_StationLocate(t.StationList[45 * start + 15 * i + j].Station, s, 2, 0);
					if (LINE[s[0]].StationList == t.StationList)s[0] = s[1];
					sprintf_s(m, sizeof(m), "%s（%s%s）", st.Name, LINE[s[0]].Name, st.Freq > 2 ? "等" : "");
				}
				else sprintf_s(m, sizeof(m), "%s", st.Name);
				H6_gotoxy(40 * i, 1 + 2 * j);
				printf("%-4d %s %-20s", t.StationList[45 * start + 15 * i + j].Kms, PROVINCE[st.Province - 1].Name, m);
			}
		}
		while (1) {
			char ct = _getch();
			if (ct == 27)return;
			if (ct == -32) {
				ct = _getch();
				if (ct == 72) {
					if (start == 0)continue;
					start--;
					break;
				}
				if (ct == 80) {
					if (start == ((t.StationNumbers - 1) / 45))continue;
					start++;
					break;
				}
			}
		}
	}
}
int main() {
	//Func7_LineLength();
	while (1) {
		H2_Initilize();
		fflush(stdin);
		fflush(stdout);
		H6_gotoxy(48, 6);
		printf("中国铁路客运里程计算器");
		H6_gotoxy(45, 8);
		printf("按0：查看使用说明");
		H6_gotoxy(45, 10);
		printf("按1：指定两车站 计算站间里程");
		H6_gotoxy(45, 12);
		printf("按2：指定线路 查询各车站里程");
		H6_gotoxy(45, 14);
		printf("按3：指定车站 查询车站的线路");
		H6_gotoxy(45, 16);
		printf("按4：将里程表输出至文件");
		H6_gotoxy(45, 18);
		printf("按Esc：关闭程序");
		switch (_getch()) {
		case '0': {
			std::thread t(Func8_HelpInfo);
			t.detach();
			break;
		}
		case '1':Func7_LineLength(); break;
		case '2':Func5_LineShow(); break;
		case '3':Func9_LineByStation(); break;
		case '4':Func1_FileMaker(); break;
		case 27:return 0;
		default:continue;
		}
	}
	
	return 0;
}