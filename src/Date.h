#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <sstream>

class Date
{
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    std::string description;

public:
    // 기본 생성자
    Date() : year(2024), month(1), day(1), hour(0), minute(0), description("") {}

    // 매개변수 생성자
    Date(int y, int m, int d, int h, int min, const std::string &desc = "")
        : year(y), month(m), day(d), hour(h), minute(min), description(desc) {}

    // 문자열로부터 생성하는 생성자 (예: "2024-10-02 14:00 - Meeting with team")
    Date(const std::string &dateString)
    {
        parseFromString(dateString);
    }

    // 문자열 파싱 함수
    void parseFromString(const std::string &dateString)
    {
        // 기본값 설정
        year = 2024;
        month = 1;
        day = 1;
        hour = 0;
        minute = 0;
        description = dateString;

        // 간단한 파싱 (예: "2024-10-02 14:00 - Meeting")
        size_t dashPos = dateString.find(" - ");
        if (dashPos != std::string::npos)
        {
            std::string datePart = dateString.substr(0, dashPos);
            description = dateString.substr(dashPos + 3);

            // 날짜와 시간 파싱
            std::stringstream ss(datePart);
            std::string token;

            // 년-월-일 파싱
            if (std::getline(ss, token, '-'))
                year = std::stoi(token);
            if (std::getline(ss, token, '-'))
                month = std::stoi(token);
            if (std::getline(ss, token, ' '))
                day = std::stoi(token);

            // 시:분 파싱
            if (std::getline(ss, token, ':'))
                hour = std::stoi(token);
            if (std::getline(ss, token))
                minute = std::stoi(token);
        }
    }

    // Getter 함수들
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }
    int getHour() const { return hour; }
    int getMinute() const { return minute; }
    std::string getDescription() const { return description; }

    // Setter 함수들
    void setYear(int y) { year = y; }
    void setMonth(int m) { month = m; }
    void setDay(int d) { day = d; }
    void setHour(int h) { hour = h; }
    void setMinute(int min) { minute = min; }
    void setDescription(const std::string &desc) { description = desc; }

    // 날짜를 문자열로 변환
    std::string toString() const
    {
        std::stringstream ss;
        ss << year << "-";
        if (month < 10)
            ss << "0";
        ss << month << "-";
        if (day < 10)
            ss << "0";
        ss << day << " ";
        if (hour < 10)
            ss << "0";
        ss << hour << ":";
        if (minute < 10)
            ss << "0";
        ss << minute;

        if (!description.empty())
        {
            ss << " - " << description;
        }

        return ss.str();
    }

    // 출력 연산자 오버로딩
    friend std::ostream &operator<<(std::ostream &os, const Date &date)
    {
        os << date.toString();
        return os;
    }

    // 비교 연산자 (날짜 정렬을 위해)
    bool operator<(const Date &other) const
    {
        if (year != other.year)
            return year < other.year;
        if (month != other.month)
            return month < other.month;
        if (day != other.day)
            return day < other.day;
        if (hour != other.hour)
            return hour < other.hour;
        return minute < other.minute;
    }
};

#endif // DATE_H
