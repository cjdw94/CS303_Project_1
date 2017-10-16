
#ifndef _DATE_H_
#define _DATE_H_

#include <stdexcept> 
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "StringTokenizer.h"
#include <sstream>
#include <iomanip>
using namespace std;
/* class Date wirtten by Mohammad Kuhail
e-mail: kuhailm@Umkc.edu
*/

const int DAYS[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //number of days in the 12 months

enum DateFormat{ Standard, US }; //Standard format is year, month, day , US format is month, day, year

class Date {

public:

	Date(DateFormat format = DateFormat::US) :format(format){}

	
	Date(string date, DateFormat format = DateFormat::US) :format(format){
		year = 1; month = 1; day = 1;
		*this = parseDate(date, format);
	}


	Date(int year, int month, int day, DateFormat format = DateFormat::US) : year(year), month(month), day(day), format(format){
		if (!valid_date(year, month, day))
			throw std::exception(" The date is not valid");
	}

	void set_format(DateFormat format){
		this->format = format;
	}

	/* return a string that represents the date*/
	string toString() const {
		switch (format){
		case DateFormat::US:
			return to_string(month) + "/" + to_string(day) + "/" + to_string(year);
		case DateFormat::Standard:
			return to_string(year) + "/" + to_string(month) + "/" + to_string(day);
		}
	}

	/* parse a date according to a given format*/
	static Date parseDate(const string& date, DateFormat format){

		String_Tokenizer st(date, "-/,");
		int year = 1, month = 1, day = 1;

		int index = 0;
		while (st.has_more_tokens()){
			switch (format){
			case DateFormat::Standard: //standard format: year month day
				switch (index){
				case 0:
					std::istringstream(st.next_token()) >> year;
					break;
				case 1:
					std::istringstream(st.next_token()) >> month;
					break;
				case 2:
					std::istringstream(st.next_token()) >> day;
					break;
				}
				break;

			case DateFormat::US: //US format: month day year
				switch (index){
				case 0:
					std::istringstream(st.next_token()) >> month;
					break;
				case 1:
					std::istringstream(st.next_token()) >> day;
					break;
				case 2:
					std::istringstream(st.next_token()) >> year;
					break;
				}
			}
			++index;
		}

		if (!valid_date(year, month, day))
			throw std::exception(" The date is not valid");

		return Date(year, month, day, format);

	}

	//this function allows the user to get a date from the user
	friend istream& operator >> (istream& in, Date& d) {
		std::string date;
		in >> date;
		d = parseDate(date, d.format);

		return in;
	}

	//checks whether this date < another date
	bool operator <(const Date& other) const{
		if (year != other.year)
			return year < other.year;

		if (month != other.month)
			return month < other.month;

		return day < other.day;
	}

	//checks if this date is valid
	void check_valid(){
		if (!valid_date(year, month, day))
			throw std::exception("The date is invalid");
	}

	//checks if a date is valid
	static bool valid_date(int year, int month, int day){

		if (year>=0 && month >= 1 && month <= 12 && day >= 1 && day <= 31){

			if (is_leap_year(year) && month == 2) // check for the leap year case
				return day <= 29;

			return day <= DAYS[month - 1];
		}
		return false;
	}

	//checks if a year is a leap year
	static bool is_leap_year(int year){

		//Algorithm is taken from Wikipedia: http://en.wikipedia.org/wiki/Leap_year
		if (year % 4 != 0)
			return false;
		else if (year % 100 != 0)
			return true;
		else if (year % 400 != 0)
			return false;
		else
			return true;
	}

	//returns the number of days in the month of this year
	int days_of_month(int month){
		return is_leap_year(year) && month == 2 ? 29 : DAYS[month - 1];
	}

	//returns the number of days in given month and year
	static int days_of_month(int month, int year){
		return is_leap_year(year) && month == 2 ? 29 : DAYS[month - 1];
	}

	//how many days are in this year?
	int days_of_year(){
		return is_leap_year(year) ? 366 : 365;
	}

	//how many days are in a given year?
	static int days_of_year(int year){
		return is_leap_year(year) ? 366 : 365;
	}

	//subtract a number of days from a date
	void subtract_days(int days){
		int reminder = days;

		while (reminder > 0){

			if (day - reminder >= 1){ // applies to all months
				day -= reminder;
				reminder = 0;
			}
			else if (month == 1){ //treat December differently
				year--;
				month = 12;
				reminder -= day;
				day = days_of_month(month);
			}
			else { //applies to all other months
				month--;
				reminder -= day;
				day = days_of_month(month);
			}
		}
		check_valid(); //check the date is valid. Maybe the year has become negative for instance
	}


	//add a number of days to this date
	void add_days(int days){

		if (days<0)
			subtract_days(-1 * days);

		int reminder = days;

		while (reminder > 0){

			int limit = days_of_month(month);

			if (reminder + day <= limit){ // applies to all months
				day += reminder;
				reminder = 0;
			}
			else if (month == 12){ //treat December differently
				year++;
				month = 1;
				reminder -= limit - day;
				day = 0;
			}
			else { //applies to all other months
				month++;
				reminder -= limit - day;
				day = 0;
			}
		}
		check_valid(); //check the date is valid. Maybe the year has become negative for instance
	}

	//difference between two dates in number of days.
	int operator - (const Date& other) const{ 
		int year_r = 0, month_r = 0, day_r = 0;

		if (this->operator<(other))
			throw std::invalid_argument("This date is < the given data");
		int year1 = year, year2 = other.year;

		while (year2 - year1 < 0){
			year_r += days_of_year(year2);
			year2++;
		}

		//year_r = (year - other.year) * 365;
		Date larger_month_date = month>other.month ? Date(year, month, day) : other;
		Date smaller_month_date = month<other.month ? Date(year, month, day) : other;
		int smaller_month = smaller_month_date.month;
		int larger_month = larger_month_date.month;

		while (smaller_month - larger_month < 0){
			month_r += days_of_month(smaller_month, smaller_month_date.year);
			smaller_month++;
		}

		if (other.month > month)
			month_r *= -1;

		day_r = day - other.day;

		return day_r + month_r + year_r;
	}


	//checks if this date is > another date
	bool operator >(const Date& other) const{
		if (year != other.year)
			return year > other.year;

		if (month != other.month)
			return month > other.month;

		return day > other.day;
	}

	//checks if this date is == another date
	bool operator ==(const Date& other) const{
		return year == other.year && month == other.month && day == other.day;
	}

	//checks if this date is != another date
	bool operator !=(const Date& other) const{
		return !(*this == other);
	}

	//checks if this date is <= another date
	bool operator <=(const Date& other) const{
		return this->operator<(other) || this->operator==(other);
	}

	//checks if this date is >= another date
	bool operator >=(const Date other) const{
		return this->operator>(other) || this->operator==(other);
	}

	int getYear() { return year; }
	int getMonth() { return month; }
	int getDay() { return day; }

	void setYear(int theYear) { year = theYear; check_valid(); }
	void setMonth(int theMonth) { month = theMonth; check_valid(); }
	void setDay(int theDay) { day = theDay; check_valid(); }


private:
	int year;
	int month;
	int day;
	DateFormat format;

};

#endif 