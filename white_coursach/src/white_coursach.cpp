#include <iostream>
#include <string>
#include <map>
#include <set>
#include <exception>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <iomanip>
using namespace std;

class Date {
public:
	int GetYear() const {
		return year;
	}
	int GetMonth() const {
		return month;
	}
	int GetDay() const {
		return day;
	}
	void ChangeDate(int new_year, int new_month, int new_day) {
		year = new_year;
		month = new_month;
		day = new_day;
	}
private:
	int year, month, day;
};
/**/
bool operator<(const Date& left, const Date& right) {
	return (left.GetYear() < right.GetYear() || (left.GetYear() == right.GetYear() && left.GetMonth() < right.GetMonth()) || (left.GetYear() == right.GetYear() && left.GetMonth() == right.GetMonth() && left.GetDay() < right.GetDay()));
}
/**/
ostream& operator<<(ostream& stream, const Date& date) {
	stream<<setfill('0')<<setw(4)<< date.GetYear();
	stream<< "-";
	stream<<setw(2)<<date.GetMonth();
	stream<< "-";
	stream<<setw(2)<<date.GetDay();
	return stream;
}

istream& operator>>(istream& stream, Date& date) {
	int year, month, day;
	char del1, del2;
	string if_wrong, tmp;
	stringstream stream2;
	stream>>if_wrong;
	stream2<<if_wrong;
	stream2 >> year >> del1 >> month >> del2 >> day; //>> EOF;
	if(!stream2 || del1!='-' || del2!='-'){
		throw invalid_argument("Wrong date format: " + if_wrong);
	}
	getline(stream2, tmp);
	if(tmp.size()>0){
		throw invalid_argument("Wrong date format: " + if_wrong);
	}
	else if(month<1 || month>12){
		throw invalid_argument("Month value is invalid: " + to_string(month));
	}
	else if(day<1 || day>31){
		throw invalid_argument("Day value is invalid: " + to_string(day));
	}
	date.ChangeDate(year, month, day);
	return stream;
}

class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		if(data.count(date)>0){
			if (data.at(date).count(event)==0) {
				data[date].insert(event);
			}
		}
		else{
			data[date].insert(event);
		}
	}
	bool DeleteEvent(const Date& date, const string& event) {
		if(data.count(date)>0 && data.at(date).count(event)>0){
			data.at(date).erase(event);
			/*auto i = find(data.at(date).begin(), data.at(date).end(), event);
			if (i != data.at(date).end()) {
				data.at(date).erase(i);
				return true;
			}*/
			return true;
		}
		return false;
	}
	int  DeleteDate(const Date& date) {
		int N = 0;
		if(data.count(date)>0){
			N=data.at(date).size();
			data.erase(date);
		}
		return N;
	}
	void Find(const Date& date) {
		if (data.count(date) > 0) {
			for (auto mipmip : data.at(date)) {
				cout<<mipmip<<endl;
			}
		}
	}
	void Print() const {
		for (auto kek : data) {
			for (auto mipmip : kek.second) {
				cout << kek.first << " " << mipmip << endl;
			}
		}
	}
private:
	map<Date, set<string>> data;
};


istream& operator>>(istream& stream, Database& db){
	Date tmp_date;
	string tmp_event;
	stream >> tmp_date >> tmp_event;
	db.AddEvent(tmp_date, tmp_event);
	return stream;
}

int main() {

	Database db;
	Date tmp_date;
	stringstream ss;
	string command, tmp_event;
	while (getline(cin, command)) {
		try{
			if(command==""){
				continue;
			}
			ss.clear();
			ss<<command;
			ss>>command;
			if (command == "Add") {
				ss >> db;
			}
			else if(command == "Del"){
				ss>>tmp_date>>tmp_event;
				if(tmp_event.empty()){
					cout<<"Deleted "<<db.DeleteDate(tmp_date)<<" events"<<endl;
				}
				else{
					if(db.DeleteEvent(tmp_date, tmp_event)){
						cout<<"Deleted successfully"<<endl;
					}
					else{
						cout<<"Event not found"<<endl;
					}
					tmp_event.clear();
				}
			}
			else if(command=="Find"){
				ss>>tmp_date;
				db.Find(tmp_date);
			}
			else if(command=="Print"){
				db.Print();
			}
			else{
				throw invalid_argument("Unknown command: " + command);
			}
		}
		catch(invalid_argument& ir){
			cout<<ir.what()<<endl;
			ss>>tmp_event;
			break;
		}
	}
	return 0;
}

/*
 *
 * #include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Date {
public:
  // конструктор выбрасывает исключение, если его аргументы некорректны
  Date(int new_year, int new_month, int new_day) {
    year = new_year;
    if (new_month > 12 || new_month < 1) {
      throw logic_error("Month value is invalid: " + to_string(new_month));
    }
    month = new_month;
    if (new_day > 31 || new_day < 1) {
      throw logic_error("Day value is invalid: " + to_string(new_day));
    }
    day = new_day;
  }

  int GetYear() const {
    return year;
  }
  int GetMonth() const {
    return month;
  }
  int GetDay() const {
    return day;
  }

private:
  int year;
  int month;
  int day;
};

// определить сравнение для дат необходимо для использования их в качестве ключей словаря
bool operator<(const Date& lhs, const Date& rhs) {
  // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
  // создадим вектор из года, месяца и дня для каждой даты и сравним их
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

// даты будут по умолчанию выводиться в нужном формате
ostream& operator<<(ostream& stream, const Date& date) {
  stream << setw(4) << setfill('0') << date.GetYear() <<
      "-" << setw(2) << setfill('0') << date.GetMonth() <<
      "-" << setw(2) << setfill('0') << date.GetDay();
  return stream;
}

class Database {
public:
  void AddEvent(const Date& date, const string& event) {
    storage[date].insert(event);
  }

  bool DeleteEvent(const Date& date, const string& event) {
    if (storage.count(date) > 0 && storage[date].count(event) > 0) {
      storage[date].erase(event);
      return true;
    }
    return false;
  }

  int DeleteDate(const Date& date) {
    if (storage.count(date) == 0) {
      return 0;
    } else {
      const int event_count = storage[date].size();
      storage.erase(date);
      return event_count;
    }
  }

  set<string> Find(const Date& date) const {
    if (storage.count(date) > 0) {
      return storage.at(date);
    } else {
      return {};
    }
  }

  void Print() const {
    for (const auto& item : storage) {
      for (const string& event : item.second) {
        cout << item.first << " " << event << endl;
      }
    }
  }

private:
  map<Date, set<string>> storage;
};

Date ParseDate(const string& date) {
  istringstream date_stream(date);
  bool ok = true;

  int year;
  ok = ok && (date_stream >> year);
  ok = ok && (date_stream.peek() == '-');
  date_stream.ignore(1);

  int month;
  ok = ok && (date_stream >> month);
  ok = ok && (date_stream.peek() == '-');
  date_stream.ignore(1);

  int day;
  ok = ok && (date_stream >> day);
  ok = ok && date_stream.eof();

  if (!ok) {
    throw logic_error("Wrong date format: " + date);
  }
  return Date(year, month, day);
}

int main() {
  try {
    Database db;

    string command_line;
    while (getline(cin, command_line)) {
      stringstream ss(command_line);

      string command;
      ss >> command;

      if (command == "Add") {

        string date_str, event;
        ss >> date_str >> event;
        const Date date = ParseDate(date_str);
        db.AddEvent(date, event);

      } else if (command == "Del") {

        string date_str;
        ss >> date_str;
        string event;
        if (!ss.eof()) {
          ss >> event;
        }
        const Date date = ParseDate(date_str);
        if (event.empty()) {
          const int count = db.DeleteDate(date);
          cout << "Deleted " << count << " events" << endl;
        } else {
          if (db.DeleteEvent(date, event)) {
            cout << "Deleted successfully" << endl;
          } else {
            cout << "Event not found" << endl;
          }
        }

      } else if (command == "Find") {

        string date_str;
        ss >> date_str;
        const Date date = ParseDate(date_str);
        for (const string& event : db.Find(date)) {
          cout << event << endl;
        }

      } else if (command == "Print") {

        db.Print();

      } else if (!command.empty()) {

        throw logic_error("Unknown command: " + command);

      }
    }
  } catch (const exception& e) {
    cout << e.what() << endl;
  }

  return 0;
}
 */
