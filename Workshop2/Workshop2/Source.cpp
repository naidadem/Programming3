#include<iostream>
using namespace std;
char *crt = "\n---------------------------------------------------------\n";

class TnameO {
	int _hours;
	int _minutes;
	int _seconds;
public:
	// Constructor without parameters
	// Constructor with 3 parameters (hours, minutess and seconds)
	// Constructor with 2 parameters (hours and minutess)
	TnameO(int hours = 0, int minutes = 0, int seconds = 0)
	{
		_hours = hours;
		_minutes = minutes;
		_seconds = seconds;
	}

		// Info function with an optional bool type parameter that determines whether tname will be printed in full format
		// (hh: mm: ss) or in abbreviated format (hh: mm). The default-no function prints in full format.
	void Info(bool fullFormat = true)
	{
		if (fullFormat)
			cout << _hours << ":" << _minutes << ":" << _seconds;
		else
			cout << _hours << ":" << _minutes;
	}

	// Add function, which as a result returns a new object of type TnameO that represents
	// the sum of tname sent as a parameter and instance this.
	TnameO Add(const TnameO &v)
	{
		int totalSeconds = (_hours + v._hours) * 3600 + (_minutes + v._minutes) * 60 + _seconds + v._seconds;
		return TnameO(totalSeconds / 3600, totalSeconds % 3600 / 60, totalSeconds % 60);
	}

	// Add function, which as a result returns a new object of type TnameO that represents
	// sum of the parameter expressed in minutes and tname of the instance this.
	TnameO Add(int minutes)
	{
		int totalSeconds = _hours * 3600 + (_minutes + minutes) * 60 + _seconds;
		return TnameO(totalSeconds / 3600, totalSeconds % 3600 / 60, totalSeconds % 60);
	}

	// CompareTo function that returns int value depending on whether TnameO instance this is before, equal or later
	// tname passed as a parameter.
	// Possible return values:
	// - 1 - TnameO instance this occurs before tname v2
	// 0 - TnameO instance this is equal to tname v2
	// 1 - TnameO instance this occurs after tname v2
	int CompareTo(const TnameO & v)
	{
		if (_hours > v._hours)
			return 1;
		if (_hours < v._hours)
			return -1;
		if (_minutes > v._minutes)
			return 1;
		if (_minutes < v._minutes)
			return -1;
		if (_seconds > v._seconds)
			return 1;
		if (_seconds < v._seconds)
			return -1;
		return 0;
	}
};


/*
You need to create an application for sharing knowledge by sharing articles among users.

The following functionalities are distinguished :
-The system administrator is in charge of managing user accounts(Name, Surname, Email, Registration Date, User Name, Password).
- Each article(Title, Content, Date, Hours, Number of Views) may contain more tags, but only belong to one area.
- All registered users can add articles.
- Application users can also leave a comment(Text, Date, Popularity) to a particular article.
*/

/*
Based on given classes implement the described system functionality. If necessary, supplement the classes with new attributes and / or functions.
*/

class date
{
	int _day;
	int _month;
	int _year;
public:

	//Potrebne konstruktor i destruktor funkcije
	date(int day = 1, int month = 1, int year = 2000)
	{
		_day = day;
		_month = month;
		_year = year;
	}

	//Info function
	void Info()
	{
		cout << _day << "." << _month << "." << _year << ".";
	}
};

class User
{
	char _name[30];
	char _userName[20];
	char* _password;
	char _userType[30]; //Administrator, Guest
public:

	//Constructor and destuctor functions needed
	User(char *name = "---", char *userName = "---", char *password = "---", char *userType = "Guest")
	{
		strcpy_s(_name, 30, name);
		strcpy_s(_userName, 20, userName);
		strcpy_s(_userType, 30, userType);
		int size = strlen(password) + 1;
		_password = new char[size];
		strcpy_s(_password, size, password);
	}
	~User()
	{
		if (_password != nullptr)
		{
			delete[] _password;
			_password = nullptr;
		}
	}

	//Function for chaning password
	void ChangePassword(char *newPassword)
	{
		if (_password != nullptr)
			delete[] _password;
		int size = strlen(newPassword) + 1;
		_password = new char[size];
		strcpy_s(_password, size, newPassword);
	}

	//Info function
	void Info()
	{
		cout << "\nUser name: " << _userName << endl;
		cout << "Name: " << _name << endl;
		cout << "User type: " << _userType << endl;
	}


	void SetUser(const User & k)
	{
		strcpy_s(_name, 30, k._name);
		strcpy_s(_userName, 20, k._userName);
		strcpy_s(_userType, 30, k._userType);
		if (_password != nullptr)
			delete[] _password;
		int size = strlen(k._password) + 1;
		_password = new char[size];
		strcpy_s(_password, size, k._password);
	}
	char * GetuserName() { return _userName; }
};

class Comment
{
	char _text[500];
	date _date;
	TnameO _time;
	int _popularity;
	User _author;
public:
	//Constructor and destuctor functions needed
	Comment(char *text, const date & date, const TnameO & time, const User & author, int popularity = 0)
	{
		strcpy_s(_text, 500, text);
		_date = date;
		_time = time;
		_author.SetUser(author);
		_popularity = popularity;
	}
	Comment() {}
	//Function for changing popularity comment
	void Setpopularity(int popularity) { _popularity = popularity; }

	//Info function
	void Info()
	{
		cout << endl << _author.GetuserName() << ": " << _text << endl;
		cout << "Date: "; _date.Info(); cout << "\tTime: "; _time.Info();
		cout << endl;
	}

	void SetUser(const Comment & k)
	{
		strcpy_s(_text, 500, k._text);
		_date = k._date;
		_time = k._time;
		_author.SetUser(k._author);
		_popularity = k._popularity;
	}
};

class Article
{
	char _title[150];
	char* _content;
	date _date;
	TnameO _time;
	int _numberOfViews;
	char* _tags[10];
	char _section[100];
	User _author;
	Comment* _Commenti;
	int _commentNumber;
public:
	Article()
	{
		strcpy_s(_title, 150, "Title");
		_content = nullptr;
		_numberOfViews = 0;
		for (int i = 0; i < 10; i++)
			_tags[i] = nullptr;
		strcpy_s(_section, 100, "Section");
		_commentNumber = 0;
		_Commenti = nullptr;
		_commentNumber = 0;
	}
	~Article()
	{
		if (_content != nullptr)
			delete[] _content;
		_content = nullptr;
		for (int i = 0; i < 10; i++)
		{
			delete[] _tags[i];
			_tags[i] = nullptr;
		}
		if (_Commenti != nullptr)
		{
			delete[] _Commenti;
			_Commenti = nullptr;
		}
	}


	void SetUser(char *title, char *content, const date & date, const TnameO & time, char *section, const User & author)
	{
		strcpy_s(_title, 150, title);
		if (_content != nullptr)
			delete[] _content;
		int sizecontent = strlen(content) + 1;
		_content = new char[sizecontent];
		strcpy_s(_content, sizecontent, content);
		_date = date;
		_time = time;
		_numberOfViews = 0;
		strcpy_s(_section, 100, section);
		_author.SetUser(author);
		_commentNumber = 0;
	}

	//Function for adding article tags
	bool AddTag(char *newTag)
	{
		for (int i = 0; i < 10; i++)
			if (_tags[i] == nullptr)
			{
				int size = strlen(newTag) + 1;
				_tags[i] = new char[size];
				strcpy_s(_tags[i], size, newTag);
				return true;
			}
		return false;
	}

	/*The function to change the content and the time of the article change. As a verification of the license, the user's name of the author of the article must be forwarded.
		The creator of the article has permission to change its content.*/

	bool ChangeContent(char * newContent, const TnameO & v, User & k)
	{
		if (strcmp(k.GetuserName(), _author.GetuserName()))
			return false;
		if (_content != nullptr)
			delete[] _content;
		int size = strlen(newContent) + 1;
		_content = new char[size];
		strcpy_s(_content, size, newContent);
		_time = v;
		strcat_s(_title, 150, " (CHANGED)");
		return true;
	}

	//Function for adding a comment
	void AddComment(const Comment &k)
	{
		if (_commentNumber == 0)
		{
			_Commenti = new Comment[_commentNumber + 1];
			_Commenti[_commentNumber++].SetUser(k);
		}
		else
		{
			Comment* newComments = new Comment[_commentNumber + 1];
			for (int i = 0; i < _commentNumber; i++)
				newComments[i].SetUser(_Commenti[i]);
			newComments[_commentNumber++].SetUser(k);
			delete[] _Commenti;
			_Commenti = newComments;
		}
	}


	//Info function
	void Info()
	{
		cout << "\nTitle: " << _title << endl;
		cout << endl << _content << endl << endl;
		cout << "\nAuthor: " << _author.GetuserName() << "\tDate: "; _date.Info(); cout << "\tTime: "; _time.Info();
		cout << "\nTags: ";
		int i = 0;
		while (_tags[i] != nullptr) cout << _tags[i++] << ", ";
		cout << crt;
		cout << "Comment (" << _commentNumber << ")\n";
		for (int i = 0; i < _commentNumber; i++)
			_Commenti[i].Info();
	}


};

int main()
{
	//adding users
	User admin("John", "admin", "password", "Administrator");
	User guest1("Bob", "bob123", "password", "Guest");
	User guest2("Marry", "ma555", "password", "Guest");
	User guest3("Anna", "anna_19", "password", "Guest");

	//creating new article
	Article noviArticle;
	char text1[1000] = "Citas o pametnim gradovima? Mastas o zivotu u gradu koji prati najnovije tehnoloske trendove, a probleme parkinga, ustede elektricne energije, zagadjenja ili procjene kvalitete tla rjesava koristenjem senzora, automatizacije i mobilnih aplikacija? Doslo je TnameO da Mostar ucinis takvim gradom! Dodji na prezentaciju 13.10.2016. u 15:00 na FIT-u, na kojem mozes SetUserti pitanja i dobiti sve potrebne informacije o programu od osnivaca City OS-a gosp. Cece Gakovica. INTERA Tehnoloski Park, u suradnji sa CityOS Foundation, a uz financijsku podrsku Ministarstva vanjskih poslova Kraljevine Norveske, otvara besplatne prijave za CityOS hackathon koji ce se odrzati 22. i 23.10.2016.godine.";
	noviArticle.SetUser("Otvorene prijave za CITYOS", text1, date(15, 10, 2016), TnameO(9, 12, 36), "Prezentacije", guest1);

	//adding tags
	noviArticle.AddTag("intera");
	noviArticle.AddTag("mostar");
	noviArticle.AddTag("pametni gradovi");
	noviArticle.AddTag("fit");

	//adding comments
	noviArticle.AddComment(Comment("Zanimljivo, sigurno cu se prijaviti", date(15, 10, 2016), TnameO(9, 39, 11), guest2));
	noviArticle.AddComment(Comment("bezveze", date(16, 10, 2016), TnameO(10, 33, 23), admin));
	noviArticle.AddComment(Comment("kako cu se prijaviti?", date(16, 10, 2016), TnameO(15, 11, 45), guest3));

	//content change
	noviArticle.ChangeContent(text1 + 28, TnameO(23, 59, 40), guest1); //IFirst sentence in text deleted
	noviArticle.ChangeContent("novi text", TnameO(23, 59, 40), guest2); //Conent isn't changed because guest2 isn't the author of the original article text

	noviArticle.Info(); //printing article

	system("pause>0");
	return 0;
}