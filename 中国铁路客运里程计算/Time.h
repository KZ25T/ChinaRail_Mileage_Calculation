/*本文件是用于便于使用时间函数的*/
/*毕竟time.h又麻烦又屑，我们都知道*/
/*菜鸡代码*/

/*	使用说明：
*	使用TIME类表示时间
*	使用D_TIME类表示时间差
*	以下所说的命令在英文情况下不区分大小写
*	且命令过多时，只解析其中的前面的若干个
*	命令错误时会弹窗
*
*	构造TIME实体时，若无命令，如 TIME t; 这样，则实体t中所有成员为0
*	若有命令，类似于 TIME t(command); 这样，
*	command若为"local"或"localtime"或"本地"或"本地时间"，
*	那么t中成员被初始化为本地时间；
*	若为"std"或"stdtime"或"标准"或"标准时间"，
*	那么t中成员被初始化为标准时间；
*	若为"new"或"新建"，则所有成员初始化为0；
*	也可以自行输入时间进行构造，
*	这样构造的时间按本地时间计算，
*	这里不会检查所有错误，因此请正确输入，
*	如果星期为-1，或省略，那么构造函数帮你计算。
*	D_TIME也差不多，看看代码就知道咋用了。
*
*	TIME和D_TIME的show()函数可以用于输出，根据输出位置重载。
*	TIME里面的命令可以是"日期"，或"时间"、"星期"及对应的英文；
*	甚至可以填上NULL；
*	D_TIME也差不多。
*	函数根据命令内容及语言进行输出。
*	
*	当然，也可以利用结构体的字段进行自定义输出。
* 
*	两种结构体都能比较时间，大于小于什么的都能用。
* 
*	听说有人想要什么夏令时之类的。
*	建议python
* 
*	定义的运算符重载，不想写交换律了，自己注意点吧。
*/

#ifndef _TIME_H_
#define _TIME_H_
#define _D_19000101 693961
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<time.h>
#include<string.h>
#include<ctype.h>
#include<Windows.h>

#ifdef __cplusplus

#ifndef _warning_4477
#define _warning_4477
#endif

#pragma warning(disable:4477)
#pragma warning(disable:6328)

int yearday(int year) {
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)return 366;
	else return 365;
}
int monthday(const int month,const int year) {
	int date[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (yearday(year) == 366)date[1] = 29;
	return date[month - 1];
}

/*C++类定义*/

class D_TIME {
public:
	long long offset = 0;
	int second = 0;
	int hour = 0;
	int minute = 0;
	int day = 0;
	int D_second = 0;
	int D_minute = 0;
	int D_hour = 0;
	int D_day = 0;

	D_TIME();
	D_TIME(long long offset);
	D_TIME(const int day, const int hour, const int minute, const int second);

	void show(const char* command);
	void show(char* const buffer, size_t size, const char* command);
	void show(FILE* ostream, const char* command);

	friend D_TIME operator+(const D_TIME& t1, const D_TIME& t2);
	friend D_TIME operator+(const D_TIME& t, const int Offset);
	friend D_TIME operator-(const D_TIME& t1, const D_TIME& t2);
	friend D_TIME operator-(const D_TIME& t, const int Offset);
	friend D_TIME operator*(const D_TIME& t, const double p);
	friend D_TIME operator/(const D_TIME& t, const double p);
	friend double operator/(const D_TIME& t1, const D_TIME& t2);

	friend bool operator<(const D_TIME& t1, const D_TIME& t2);
	friend bool operator>(const D_TIME& t1, const D_TIME& t2);
	friend bool operator<=(const D_TIME& t1, const D_TIME& t2);
	friend bool operator>=(const D_TIME& t1, const D_TIME& t2);
	friend bool operator==(const D_TIME& t1, const D_TIME& t2);
	friend bool operator!=(const D_TIME& t1, const D_TIME& t2);
};

D_TIME::D_TIME() {
	return;
};
D_TIME::D_TIME(long long Offset) {
	offset = Offset;
	Offset = Offset >= 0 ? Offset : -Offset;
	D_second = (int)offset;
	D_minute = (int)(Offset / 60) * (offset > 0 ? 1 : -1);
	D_hour = (int)(D_minute / 60) * (offset > 0 ? 1 : -1);
	D_day = (int)(D_hour / 24) * (offset > 0 ? 1 : -1);
	second = Offset % 60;
	minute = (Offset / 60) % 60;
	hour = (Offset / 3600) % 24;
	day = (int)(Offset / (3600 * 24));
	return;
}
D_TIME::D_TIME(const int Day, const int Hour, const int Minute, const int Second) {
	if (Day < 0 || Hour < 0 || Hour >= 24 || Minute < 0 || Minute >= 60 || Second < 0 || Second >= 60) {
		MessageBoxA(NULL, "错误：无效时间！", "错误提醒", MB_OK | MB_ICONERROR);
		return;
	}
	offset = ((long long int)(((long long int)Day * 24) + Hour) * 60 + Minute) * 60 + second;
	D_second = (int)offset;
	D_minute = (int)(offset / 60);
	D_hour = (int)(D_minute / 60);
	D_day = (int)(D_hour / 24);
	second = Second;
	minute = Minute;
	hour = Hour;
	day = Day;
	return;
}

void D_TIME::show(const char* command) {
	char cmd[20] = { 0 };
	if (command != NULL)sscanf_s(command, "%s", cmd, sizeof(cmd));
	_strlwr_s(cmd);
	if (command == NULL || strcmp(cmd, "标准")==0)printf("%02d天%02d时%02d分%02d秒", day, hour, minute, second);
	else if (strcmp(cmd, "std")==0)printf("%02d day(s),%02d hour(s),%02d minute(s),%02d second(s)", day, hour, minute, second);
	else if (strcmp(cmd, "非零")==0) {
		if (day != 0)printf("%02d天%02d时%02d分%02d秒", day, hour, minute, second);
		else if (day == 0 && hour != 0)printf("%02d时%02d分%02d秒", hour, minute, second);
		else if (day == 0 && hour == 0 && minute != 0)printf("%02d分%02d秒", minute, second);
		else printf("%02d秒", second);
	}
	else if (strcmp(cmd, "-0")==0) {
		if (day != 0)printf("%02d day(s),%02d hour(s),%02d minute(s),%02d second(s)", day, hour, minute, second);
		else if (day == 0 && hour != 0)printf("%02d hour(s),%02d minute(s),%02d second(s)", hour, minute, second);
		else if (day == 0 && hour == 0 && minute != 0)printf("%02d minute(s),%02d second(s)", minute, second);
		else printf("%02d second(s)", second);
	}
	else MessageBoxA(NULL, "错误：无法解析的命令！", "错误提醒", MB_OK | MB_ICONERROR);
	return;
}
void D_TIME::show(char* const buffer, size_t size, const char* command) {
	memset(buffer, 0, size);
	char cmd[20] = { 0 };
	if (command != NULL)sscanf_s(command, "%s", cmd, sizeof(cmd));
	_strlwr_s(cmd);
	if (command == NULL || strcmp(cmd, "标准") == 0)sprintf_s(buffer, size, "%02d天%02d时%02d分%02d秒", day, hour, minute, second);
	else if (strcmp(cmd, "std") == 0)sprintf_s(buffer, size, "%02d day(s),%02d hour(s),%02d minute(s),%02d second(s)", day, hour, minute, second);
	else if (strcmp(cmd, "非零") == 0) {
		if (day != 0)sprintf_s(buffer, size, "%02d天%02d时%02d分%02d秒", day, hour, minute, second);
		else if (day == 0 && hour != 0)sprintf_s(buffer, size, "%02d时%02d分%02d秒", hour, minute, second);
		else if (day == 0 && hour == 0 && minute != 0)sprintf_s(buffer, size, "%02d分%02d秒", minute, second);
		else sprintf_s(buffer, size, "%02d秒", second);
	}
	else if (strcmp(cmd, "-0") == 0) {
		if (day != 0)sprintf_s(buffer, size, "%02d day(s),%02d hour(s),%02d minute(s),%02d second(s)", day, hour, minute, second);
		else if (day == 0 && hour != 0)sprintf_s(buffer, size, "%02d hour(s),%02d minute(s),%02d second(s)", hour, minute, second);
		else if (day == 0 && hour == 0 && minute != 0)sprintf_s(buffer, size, "%02d minute(s),%02d second(s)", minute, second);
		else sprintf_s(buffer, size, "%02d second(s)", second);
	}
	else MessageBoxA(NULL, "错误：无法解析的命令！", "错误提醒", MB_OK | MB_ICONERROR);
	return;
}
void D_TIME::show(FILE* ostream, const char* command) {
	char cmd[20] = { 0 };
	if (command != NULL)sscanf_s(command, "%s", cmd, sizeof(cmd));
	_strlwr_s(cmd);
	if (command == NULL || strcmp(cmd, "标准") == 0)fprintf(ostream, "%02d天%02d时%02d分%02d秒", day, hour, minute, second);
	else if (strcmp(cmd, "std") == 0)fprintf(ostream, "%02d day(s),%02d hour(s),%02d minute(s),%02d second(s)", day, hour, minute, second);
	else if (strcmp(cmd, "非零") == 0) {
		if (day != 0)fprintf(ostream, "%02d天%02d时%02d分%02d秒", day, hour, minute, second);
		else if (day == 0 && hour != 0)fprintf(ostream, "%02d时%02d分%02d秒", hour, minute, second);
		else if (day == 0 && hour == 0 && minute != 0)fprintf(ostream, "%02d分%02d秒", minute, second);
		else fprintf(ostream, "%02d秒", second);
	}
	else if (strcmp(cmd, "-0") == 0) {
		if (day != 0)fprintf(ostream, "%02d day(s),%02d hour(s),%02d minute(s),%02d second(s)", day, hour, minute, second);
		else if (day == 0 && hour != 0)fprintf(ostream, "%02d hour(s),%02d minute(s),%02d second(s)", hour, minute, second);
		else if (day == 0 && hour == 0 && minute != 0)fprintf(ostream, "%02d minute(s),%02d second(s)", minute, second);
		else fprintf(ostream, "%02d second(s)", second);
	}
	else MessageBoxA(NULL, "错误：无法解析的命令！", "错误提醒", MB_OK | MB_ICONERROR);
	return;
}

D_TIME operator+(const D_TIME& t1, const D_TIME& t2) {
	return D_TIME(t1.offset + t2.offset);
}
D_TIME operator+(const D_TIME& t, const int Offset) {
	return D_TIME(t.offset + Offset);
}
D_TIME operator-(const D_TIME& t1, const D_TIME& t2) {
	return D_TIME(t1.offset - t2.offset);
}
D_TIME operator-(const D_TIME& t, const int Offset) {
	return D_TIME(t.offset - Offset);
}
D_TIME operator*(const D_TIME& t, const double p) {
	return D_TIME((long long)(t.offset * p));
}
D_TIME operator/(const D_TIME& t, const double p) {
	return D_TIME((long long)(t.offset / p));
}
double operator/(const D_TIME& t1, const D_TIME& t2) {
	return (double)t1.offset / t2.offset;
}

bool operator<(const D_TIME& t1, const D_TIME& t2) {
	return t1.offset < t2.offset;
}
bool operator>(const D_TIME& t1, const D_TIME& t2) {
	return t1.offset > t2.offset;
}
bool operator<=(const D_TIME& t1, const D_TIME& t2) {
	return t1.offset <= t2.offset;
}
bool operator>=(const D_TIME& t1, const D_TIME& t2) {
	return t1.offset >= t2.offset;
}
bool operator==(const D_TIME& t1, const D_TIME& t2) {
	return t1.offset == t2.offset;
}
bool operator!=(const D_TIME& t1, const D_TIME& t2) {
	return t1.offset != t2.offset;
}

class TIME {
private:
	long long timep = 0;
	int status = 0;
public:
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
	int weekday = 0;

	TIME();
	TIME(const char* command);
	TIME(const int year, const int month, const int day, const int hour, const int minute, const int second, const int weekday = -1);

	void show(const char* command);
	void show(char* const buffer, size_t size, const char* command);
	void show(FILE* ostream, const char* command);
	void remake(int IfStd);

	friend D_TIME operator-(const TIME& t1, const TIME& t2);
	friend TIME operator+(const TIME& t, const D_TIME& add);
	friend TIME operator+(const TIME& time, const long long int offset);
	friend TIME operator-(const TIME& t, const D_TIME& add);
	friend TIME operator-(const TIME& time, const long long int offset);

	friend bool operator>(const TIME& t1, const TIME& t2);
	friend bool operator<(const TIME& t1, const TIME& t2);
	friend bool operator>=(const TIME& t1, const TIME& t2);
	friend bool operator<=(const TIME& t1, const TIME& t2);
	friend bool operator==(const TIME& t1, const TIME& t2);
	friend bool operator!=(const TIME& t1, const TIME& t2);
};

TIME::TIME() {
	return;
};
TIME::TIME(const char* command) {
	char cmd[40] = { 0 };
	if (command != NULL)sscanf_s(command, "%s", cmd, sizeof(cmd));
	_strlwr_s(cmd);
	struct tm TM, * tp = &TM;
	time(&timep);
	if (command == NULL ||
		strcmp(cmd, "local") == 0 ||
		strcmp(cmd, "localtime") == 0 ||
		strcmp(cmd, "本地") == 0 ||
		strcmp(cmd, "本地时间") == 0) {
		localtime_s(tp, &timep);
		status = 0;
	}
	else if (strcmp(cmd, "std") == 0 ||
		strcmp(cmd, "stdtime") == 0 ||
		strcmp(cmd, "标准") == 0 ||
		strcmp(cmd, "标准时间") == 0) {
		//tp = gmtime(&timep);
		gmtime_s(tp, &timep);
		status = 1;
	}
	else if (strcmp(cmd, "new") == 0 || strcmp(cmd, "新建") == 0) {
		timep = year = month = day = hour = minute = second = weekday = 0;
		return;
	}
	else {
		MessageBoxA(NULL, "错误：无法解析的命令！", "错误提醒", MB_OK | MB_ICONERROR);
		return;
	}
	year = tp->tm_year + 1900;
	month = tp->tm_mon + 1;
	day = tp->tm_mday;
	hour = tp->tm_hour;
	minute = tp->tm_min;
	second = tp->tm_sec;
	weekday = tp->tm_wday;
	return;
}
TIME::TIME(const int Year, const int Month, const int Day, const int Hour, const int Minute, const int Second, const int Weekday) {
	if (Year <= 0 || Month <= 0 || Month > 12 || Day <= 0 || Day > monthday(Month, Year) || 
		Hour < 0 || Hour >= 24 || Minute < 0 || Minute >= 60 || Second < 0 || Second >= 60) {
		MessageBoxA(NULL, "错误：无效日期！", "错误提醒", MB_OK | MB_ICONERROR);
		return;
	}
	
	/*计算时间戳*/
	unsigned int year_to_day = 0;
	unsigned int mon_to_day = 0;
	unsigned int day_to_day = 0;
	/*把1月和2月当作上一年年底的最后两个月，让3月作为一年的第一个月*/
	int _mon = Month;
	int _year = Year;
	if ((_mon -= 2) <= 0){
		_mon += 12;
		_year -= 1;
	}
	year_to_day = (_year - 1) * 365 + _year / 4 - _year / 100 + _year / 400; //计算给出的年数到公元元年的天数
	mon_to_day = (367 * _mon / 12) - 30 + 59; //计算给出的月数转换成天数
	day_to_day = Day - 1; //当天不能计算上去，所以要减1
	timep = (long long int)(((((long long int)((long long int)year_to_day + mon_to_day + day_to_day - 719162) * 24 + Hour) * 60 + Minute) * 60) + Second) - 28800;
	struct tm TM, * tp = &TM;
	localtime_s(tp, &timep);
	if (Weekday != -1 && tp->tm_wday != Weekday) {
		MessageBoxA(NULL, "错误：星期与日期不匹配！", "错误提醒", MB_OK | MB_ICONERROR);
		timep = 0;
		return;
	}
	status = 0;
	year = Year;
	month = Month;
	day = Day;
	hour = Hour;
	minute = Minute;
	second = Second;
	weekday = tp->tm_wday;
	return;
}

void TIME::show(const char* command) {
	if (command != NULL) {
		char argc[3][20] = { 0 };
		int argv = sscanf_s(command, "%s%s%s", argc[0], sizeof(argc[0]), argc[1], sizeof(argc[1]), argc[2], sizeof(argc[2]));
		for (int i = 0; i < argv; i++) {
			_strlwr_s(argc[i]);
			if (i != 0)putchar(' ');
			if (strcmp(argc[i], "日期") == 0)printf("%04d年%02d月%02d日", year, month, day);
			else if (strcmp(argc[i], "day") == 0 || strcmp(argc[i], "date") == 0)printf("%04d-%02d-%02d", year, month, day);
			else if (strcmp(argc[i], "时间") == 0)printf("%02d时%02d分%02d秒", hour, minute, second);
			else if (strcmp(argc[i], "time") == 0)printf("%02d:%02d:%02d", hour, minute, second);
			else if (strcmp(argc[i], "星期") == 0)switch (weekday) {
			case 0:printf("星期日"); break;
			case 1:printf("星期一"); break;
			case 2:printf("星期二"); break;
			case 3:printf("星期三"); break;
			case 4:printf("星期四"); break;
			case 5:printf("星期五"); break;
			case 6:printf("星期六"); break;
			}
			else if (strcmp(argc[i], "weekday") == 0 || strcmp(argc[i], "week") == 0)switch (weekday) {
			case 0:printf("Sunday"); break;
			case 1:printf("Monday"); break;
			case 2:printf("Tuesday"); break;
			case 3:printf("Wednesday"); break;
			case 4:printf("Thursday"); break;
			case 5:printf("Friday"); break;
			case 6:printf("Saturday"); break;
			}
			else MessageBoxA(NULL, "错误：无法解析的命令！", "错误提醒", MB_OK | MB_ICONERROR);
		}
	}
	else printf("%04d年%02d月%02d日 %02d时%02d分%02d秒", year, month, day, hour, minute, second);
	return;
}
void TIME::show(char* const buffer, size_t size, const char* command) {
	memset(buffer, 0, size);
	if (command != NULL) {
		char argc[3][20] = { 0 };
		int argv = sscanf_s(command, "%s%s%s", argc[0], sizeof(argc[0]), argc[1], sizeof(argc[1]), argc[2], sizeof(argc[2]));
		for (int i = 0; i < argv; i++) {
			_strlwr_s(argc[i]);
			if (i != 0)strcpy_s(buffer + strlen(buffer), size - strlen(buffer), " ");
			if (strcmp(argc[i], "日期") == 0)sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "%04d年%02d月%02d日", year, month, day);
			else if (strcmp(argc[i], "day") == 0 || strcmp(argc[i], "date") == 0)sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "%04d-%02d-%02d", year, month, day);
			else if (strcmp(argc[i], "时间") == 0)sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "%02d时%02d分%02d秒", hour, minute, second);
			else if (strcmp(argc[i], "time") == 0)sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "%02d:%02d:%02d", hour, minute, second);
			else if (strcmp(argc[i], "星期") == 0)switch (weekday) {
			case 0:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "星期日"); break;
			case 1:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "星期一"); break;
			case 2:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "星期二"); break;
			case 3:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "星期三"); break;
			case 4:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "星期四"); break;
			case 5:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "星期五"); break;
			case 6:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "星期六"); break;
			}
			else if (strcmp(argc[i], "weekday") == 0 || strcmp(argc[i], "week") == 0)switch (weekday) {
			case 0:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "Sunday"); break;
			case 1:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "Monday"); break;
			case 2:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "Tuesday"); break;
			case 3:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "Wednesday"); break;
			case 4:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "Thursday"); break;
			case 5:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "Friday"); break;
			case 6:sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "Saturday"); break;
			}
			else MessageBoxA(NULL, "错误：无法解析的命令！", "错误提醒", MB_OK | MB_ICONERROR);
		}
	}
	else sprintf_s(buffer + strlen(buffer), size - strlen(buffer), "%04d年%02d月%02d日 %02d时%02d分%02d秒", year, month, day, hour, minute, second);
	return;
}
void TIME::show(FILE* ostream, const char* command) {
	if (command != NULL) {
		char argc[3][20] = { 0 };
		int argv = sscanf_s(command, "%s%s%s", argc[0], sizeof(argc[0]), argc[1], sizeof(argc[1]), argc[2], sizeof(argc[2]));
		for (int i = 0; i < argv; i++) {
			_strlwr_s(argc[i]);
			if (i != 0)fputc(' ', ostream);
			if (strcmp(argc[i], "日期") == 0)fprintf(ostream, "%04d年%02d月%02d日", year, month, day);
			else if (strcmp(argc[i], "day") == 0 || strcmp(argc[i], "date") == 0)fprintf(ostream, "%04d-%02d-%02d", year, month, day);
			else if (strcmp(argc[i], "时间") == 0)fprintf(ostream, "%02d时%02d分%02d秒", hour, minute, second);
			else if (strcmp(argc[i], "time") == 0)fprintf(ostream, "%02d:%02d:%02d", hour, minute, second);
			else if (strcmp(argc[i], "星期") == 0)switch (weekday) {
			case 0:fprintf(ostream, "星期日"); break;
			case 1:fprintf(ostream, "星期一"); break;
			case 2:fprintf(ostream, "星期二"); break;
			case 3:fprintf(ostream, "星期三"); break;
			case 4:fprintf(ostream, "星期四"); break;
			case 5:fprintf(ostream, "星期五"); break;
			case 6:fprintf(ostream, "星期六"); break;
			}
			else if (strcmp(argc[i], "weekday") == 0 || strcmp(argc[i], "week") == 0)switch (weekday) {
			case 0:fprintf(ostream, "Sunday"); break;
			case 1:fprintf(ostream, "Monday"); break;
			case 2:fprintf(ostream, "Tuesday"); break;
			case 3:fprintf(ostream, "Wednesday"); break;
			case 4:fprintf(ostream, "Thursday"); break;
			case 5:fprintf(ostream, "Friday"); break;
			case 6:fprintf(ostream, "Saturday"); break;
			}
			else MessageBoxA(NULL, "错误：无法解析的命令！", "错误提醒", MB_OK | MB_ICONERROR);
		}
	}
	else fprintf(ostream, "%04d年%02d月%02d日 %02d时%02d分%02d秒", year, month, day, hour, minute, second);
	return;
}
void TIME::remake(int IfStd) {
	struct tm TM, * tp = &TM;
	if (IfStd) {
		status = 1;
		gmtime_s(tp, &timep);
	}
	else {
		status = 0;
		localtime_s(tp, &timep);
	}
	year = tp->tm_year + 1900;
	month = tp->tm_mon + 1;
	day = tp->tm_mday;
	hour = tp->tm_hour;
	minute = tp->tm_min;
	second = tp->tm_sec;
	weekday = tp->tm_wday;
	return;
}

D_TIME operator-(const TIME& t1, const TIME& t2) {
	return D_TIME(t1.timep - t2.timep);
}
TIME operator+(const TIME& time, const D_TIME& add) {
	TIME t;
	t.timep = time.timep + add.offset;
	t.status = time.status;
	struct tm TM, * tp = &TM;
	time.status == 0 ? localtime_s(tp, &t.timep) : gmtime_s(tp, &t.timep);
	t.year = tp->tm_year + 1900;
	t.month = tp->tm_mon + 1;
	t.day = tp->tm_mday;
	t.hour = tp->tm_hour;
	t.minute = tp->tm_min;
	t.second = tp->tm_sec;
	t.weekday = tp->tm_wday;
	return t;
}
TIME operator+(const TIME& time, const long long int Offset) {
	TIME t;
	t.timep = time.timep + Offset;
	t.status = time.status;
	struct tm TM, * tp = &TM;
	time.status == 0 ? localtime_s(tp, &t.timep) : gmtime_s(tp, &t.timep);
	t.year = tp->tm_year + 1900;
	t.month = tp->tm_mon + 1;
	t.day = tp->tm_mday;
	t.hour = tp->tm_hour;
	t.minute = tp->tm_min;
	t.second = tp->tm_sec;
	t.weekday = tp->tm_wday;
	return t;
}
TIME operator-(const TIME& time, const D_TIME& add) {
	TIME t;
	t.timep = time.timep - add.offset;
	t.status = time.status;
	struct tm TM, * tp = &TM;
	time.status == 0 ? localtime_s(tp, &t.timep) : gmtime_s(tp, &t.timep);
	t.year = tp->tm_year + 1900;
	t.month = tp->tm_mon + 1;
	t.day = tp->tm_mday;
	t.hour = tp->tm_hour;
	t.minute = tp->tm_min;
	t.second = tp->tm_sec;
	t.weekday = tp->tm_wday;
	return t;
}
TIME operator-(const TIME& time, const long long int Offset) {
	TIME t;
	t.timep = time.timep - Offset;
	t.status = time.status;
	struct tm TM, * tp = &TM;
	time.status == 0 ? localtime_s(tp, &t.timep) : gmtime_s(tp, &t.timep);
	t.year = tp->tm_year + 1900;
	t.month = tp->tm_mon + 1;
	t.day = tp->tm_mday;
	t.hour = tp->tm_hour;
	t.minute = tp->tm_min;
	t.second = tp->tm_sec;
	t.weekday = tp->tm_wday;
	return t;
}

bool operator>(const TIME& t1, const TIME& t2) {
	return t1.timep > t2.timep;
}
bool operator<(const TIME& t1, const TIME& t2) {
	return t1.timep < t2.timep;
}
bool operator>=(const TIME& t1, const TIME& t2) {
	return t1.timep >= t2.timep;
}
bool operator<=(const TIME& t1, const TIME& t2) {
	return t1.timep <= t2.timep;
}
bool operator==(const TIME& t1, const TIME& t2) {
	return t1.timep == t2.timep;
}
bool operator!=(const TIME& t1, const TIME& t2) {
	return t1.timep != t2.timep;
}

void TestShow() {
	//这玩意可以测试输出，展示了这个头文件的部分功能
	printf("现在我们开始来测试头文件\"Time.h\"：");
	printf("\n定义当前时间t：");
	TIME t("本地时间");
	printf("\n输出本地时间1：\t\t");
	t.show("日期 时间 星期");
	printf("\n输出本地时间2：\t\t");
	t.show(stdout, "日期 时间");
	printf("\n输出本地时间3：\t\t");
	char s[60];
	t.show(s, sizeof(s), "日期 时间 星期");
	printf(s);
	printf("\n输出本地时间4：\t\t");
	t.show(NULL);
	printf("\n英文输出本地时间1：\t");
	t.show("day time week");
	printf("\n英文输出本地时间2：\t");
	t.show("date weekday time");
	printf("\n+1s：\t\t\t");
	t = t + 1;
	t.show("日期 时间 星期");
	printf("\n-1s：\t\t\t");
	t = t - 1;
	t.show("日期 时间 星期");
	printf("\n改为格林尼治时间(UTC)：\t");
	t.remake(1);
	t.show("日期 时间 星期");
	printf("\n改回来：\t\t");
	t.remake(0);
	t.show("日期 时间 星期");
	printf("\n还可以重新赋值：\t");
	t = TIME(2021, 1, 1, 0, 0, 0);
	t.show("日期 星期 时间");
	printf("\n还可以直接构造时间：\t");
	TIME t2(2022, 1, 1, 0, 0, 0);
	t2.show("日期 星期 时间");
	printf("\n时间之间可以比较大小：\t");
	t2 > t ? t2.show(NULL) : t.show(NULL);
	printf("比");
	printf("%s", (t2 < t ? t2.show(NULL) : t.show(NULL), "更大"));
	printf("\n也可以判断是否相等：\t");
	printf("两个时间%s", (t2 == t ? "相等" : "不相等"));
	printf("\n两个时间相减，计算2021年长度：\t");
	D_TIME dt = t2 - t;
	dt.show("标准");
	printf("\n通过类的字段得到：\t\t2021年共有%lld秒", dt.offset);
	printf("\n倒着减，输出的时间间隔为绝对值：");
	(t - t2).show(NULL);
	printf("\n但是其时间偏移量为负值：\t%lld", (t - t2).offset);
	printf("\n时间间隔也可以直接加上秒数：\t");
	(dt + 3661).show(NULL);
	printf("\n2021年的30%%是这么长：\t\t");
	(dt * 0.3).show("标准");
	printf("\n2021年的是一半是这么长：\t");
	(dt / 2).show("标准");
	printf("\n两个时间段的长度比值为：\t%f", (dt * 0.3) / (dt * 0.5));
	printf("\n此时的时间是：\t\t\t");
	t2 = t + dt * 0.5;
	t2.show("日期 时间 星期");
	printf("\n这个时间距离国庆还有：\t\t");
	(TIME(2021, 10, 1, 0, 0, 0) - t2).show("标准");
	printf("\n2021.9.17，G1次列车(北京南-上海，07:00-11:29)的全程用时为：\t");
	(TIME(2021, 9, 17, 11, 29, 00) - TIME(2021, 9, 17, 07, 00, 00)).show("非零");
	printf("\n测试完毕！");
	return;
}
#ifdef _warning_4477
#undef _warning_4477
#endif

#else

/*C语言*/
/*懒得写了，就这样吧*/
/*如果你愿意的话你自己写吧，我必将万分感激*/

#endif

#endif