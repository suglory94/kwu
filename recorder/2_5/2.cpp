#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

class Time {
private:
	int hour;
	int minute;
	int second;
public:
	Time() {
		hour = 0;
		minute = 0;
		second = 0;
	}
	
	Time(int hour, int min, int sec) {
		setTime(hour, min, sec);
	}

	void setTime(int hour, int min, int sec) {
		this->hour = hour;
		this->minute = min;
		this->second = sec;
	}

	void addTime(int sec) {
		second += sec;
		minute += second / 60;
		second %= 60;
		hour += minute / 60;
		minute %= 60;
		hour %= 24;
	}

	void printTime() {
		cout << hour << ":" << minute << ":" << second << endl;
	}
};

class Korea : public Time {
public:
	Korea(int h, int m, int s) {
		setTime(h, m, s);
	}
};

class WashingtonDC : public Time {
public:
	WashingtonDC(int h, int m, int s) {
		setTime((h + 24 - 14) % 24, m, s);
	}
};

class Paris : public Time {
public:
	Paris(int h, int m, int s) {
		setTime((h + 24 - 8) % 24, m, s);
	}
};

class GreenwichObservatory : public Time {
public:
	GreenwichObservatory(int h, int m, int s) {
		setTime((h + 24 - 9) % 24, m, s);
	}
};

int main() {
	time_t timer;
	struct tm* t;
	timer = time(NULL);
	t = localtime(&timer);
	string menu;
	Time time1;
	time1.setTime(t->tm_hour, t->tm_min, t->tm_sec);
	Korea k(t->tm_hour, t->tm_min, t->tm_sec);
	WashingtonDC w(t->tm_hour, t->tm_min, t->tm_sec);
	Paris p(t->tm_hour, t->tm_min, t->tm_sec);
	GreenwichObservatory g(t->tm_hour, t->tm_min, t->tm_sec);

	cout << "Korea Time : ";
	k.printTime();
	cout << "Washington DC Time : ";
	w.printTime();
	cout << "Paris Time : ";
	p.printTime();
	cout << "Greenwich Observatory Time : ";
	g.printTime();

	return 0;
}