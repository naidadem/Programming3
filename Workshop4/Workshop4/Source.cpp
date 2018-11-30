#include <iostream>
#include <conio.h>
using namespace std;

class Member
{
	const int _memberID; //automatically generate and prevent duplication
	char*_nameSurname;
	char _phoneNumber[20];
	int _numberOfRentedBooks;
	static int _memberCounter;

	static const char * _library;
public:
	Member(char* nameSurname = "<Not set>", char* phoneNumber = "<Not set>") : _memberID(_memberCounter++)
	{
		int vel = strlen(nameSurname) + 1;
		_nameSurname = new char[vel];
		strcpy_s(_nameSurname, vel, nameSurname);

		strncpy_s(_phoneNumber, phoneNumber, _TRUNCATE);

		_numberOfRentedBooks = 0;
	}
	~Member()
	{
		if (_nameSurname != nullptr)
		{
			delete[] _nameSurname;
			_nameSurname = nullptr;
		}
	}

	char * GetName() { return _nameSurname; }

	friend ostream & operator <<(ostream &, const Member &);

	void IncreaseTheNumberOfRentedBooks()
	{
		_numberOfRentedBooks += 1;
	}

	void DecreaseTheNumberOfRentedBooks()
	{
		_numberOfRentedBooks -= 1;
	}

	Member & operator = (const Member & right)
	{
		if (this != &right)
		{
			if (_nameSurname != nullptr)
			{
				delete[] _nameSurname;
				_nameSurname = nullptr;
			}

			int size = strlen(right._nameSurname) + 1;
			_nameSurname = new char[size];
			strcpy_s(_nameSurname, size, right._nameSurname);

			strcpy_s(_phoneNumber, right._phoneNumber);

			_numberOfRentedBooks = right._numberOfRentedBooks;
		}
		return *this;
	}

	friend bool operator == (const Member &, const Member &);
	friend bool operator == (const Member &, const int);

	int GetMemberID()const { return _memberID; }
	int GetNumberOfRentedBooks()const { return _numberOfRentedBooks; }

	static void ResetMmberCounter()
	{
		_memberCounter = 1;
	}
};

bool operator == (const Member & left, const Member & right)
{
	return (left._memberID == right._memberID);
}
bool operator == (const Member &left, const int number)
{
	return (left._memberID == number);
}

ostream & operator <<(ostream &printing, const Member & obj)
{
	if (obj._nameSurname != nullptr)
		printing << obj._nameSurname;
	printing << " (" << obj._memberID << ")" << endl;
	printing << "Phone number: " << obj._phoneNumber << endl;
	printing << "Number of rented books: " << obj._numberOfRentedBooks << endl;

	return printing;
}

int Member::_memberCounter = 1;
const char * Member::_library = "Library of University of Mostar";

class Book
{
	char _ISBN[20];
	char* _title;
	int _yearOfPublishing;
	char* _author;
	char _genre[30];
	Member* _Member; //Podaci o članu kod kojeg se Book trenutno nalazi
	int _numberOfBusynessDays;

	static bool CheckISBN(char* ISBN)
	{
		int length = strlen(ISBN);

		if (length < 17)
			return false;
		else
		{
			for (size_t i = 0; i < length; i++)
				if (isalpha(ISBN[i]))
					return false;
		}
		int hyphenCounter = 0;

		for (size_t i = 0; i < length; i++)
			if (ISBN[i] == '-')
				hyphenCounter += 1;
		if (hyphenCounter != 4)
			return false;

		int numberCounter = 0;

		for (size_t i = 0; i < length; i++)
		{
			if (isdigit(ISBN[i]))
				numberCounter += 1;
			if (i == 3 || i == 7 || i == 11)
			{
				if (numberCounter == 3)
					numberCounter = 0;
				else
					return false;
			}
			else if (i == 17)
			{
				if (numberCounter != 1)
					return false;
			}
		}
		//The function is checking validaty of the ISBN number of the book
		//13-digit ISBN numbers are supported in the format 978-953-220-545-9
		//Rule: The number should contain 4 characters '-', without the letters (upper and lower letters)

		//In addition, check later that three characters are written between the individual characters '-' and the last one is only one
		return true;
	}

public:
	//Needed constructor and destructor functions
	//In constructor implementation, check the validity of the ISBN number, and if it is incorrect, set all attributes to default values.
	//Book initially not in charge
	Book(char * isbn, char * title, char * author, char * genre, int yearOfPublishing)
	{
		if (CheckISBN(isbn))
		{
			strcpy_s(_ISBN, isbn);

			int size = strlen(title) + 1;
			_title = new char[size];
			strcpy_s(_title, size, title);

			size = strlen(author) + 1;
			_author = new char[size];
			strcpy_s(_author, size, author);

			_numberOfBusynessDays = 0;

			_Member = nullptr;

			strcpy_s(_genre, genre);

			_yearOfPublishing = yearOfPublishing;
		}
		else
		{
			strcpy_s(_ISBN, "N/A");

			_title = nullptr;

			_author = nullptr;

			_numberOfBusynessDays = -1;

			_Member = nullptr;

			strcpy_s(_genre, "N/A");

			_yearOfPublishing = -1;
		}
	}
	Book()
	{
		strcpy_s(_ISBN, "N/A");
		strcpy_s(_genre, "N/A");
		_title = nullptr;
		_author = nullptr;
		_Member = nullptr;
		_numberOfBusynessDays = -1;
		_yearOfPublishing = -1;
	}
	~Book()
	{
		if (_title != nullptr)
		{
			delete[] _title;
			_title = nullptr;
		}
		if (_author != nullptr)
		{
			delete[] _author;
			_author = nullptr;
		}
		/*if (_Member != nullptr)
		{
		delete _Member;
		_Member = nullptr;
		}*/
	}

	//The ReserveBook function that needs to set up information about the member at which book is currently located
	//Make sure that the original member type of the Member increases the number of rented books for one
	void ReserveBook(Member & Member)
	{
		if (!CheckIfReserved())
		{
			Member.IncreaseTheNumberOfRentedBooks();
			_Member = &Member;
			_numberOfBusynessDays++;
		}
	}
	//Function of the Book of books that describes the book as free, or removes the information of the member to which it is currently located
	void ReleaseBook()
	{
		if (CheckIfReserved())
		{
			_Member->DecreaseTheNumberOfRentedBooks();
			_Member = nullptr;
			_numberOfBusynessDays = 0;
		}
	}
	//The CheckIfReserved function returns the "true" logical value if the book is in charge, otherwise returns "false"
	bool CheckIfReserved()const { return (_Member != nullptr); }

	friend ostream& operator << (ostream &, const Book &);

	friend bool operator !=(const Book &, const Book &);

	friend bool operator ==(const Book &, const Book &);

	Book & operator =(const Book & right)
	{
		if (this != &right)
		{
			if (this->_title != nullptr)
				delete[] _title;
			if (this->_author != nullptr)
				delete[]_author;

			int size = strlen(right._title) + 1;
			_title = new char[size];
			strcpy_s(_title, size, right._title);

			size = strlen(right._author) + 1;
			_author = new char[size];
			strcpy_s(_author, size, right._author);

			_yearOfPublishing = right._yearOfPublishing;

			strcpy_s(_genre, right._genre);
			strcpy_s(_ISBN, right._ISBN);

			_numberOfBusynessDays = right._numberOfBusynessDays;

			_Member = right._Member;
		}
		return *this;
	}

	int GetNumberOfBusyness()const { return _numberOfBusynessDays; }

	const char * GetISBN()const { return _ISBN; }
	const char * GetAuthor()const { return _author; }
	const char * GetTitle()const { return _title; }

	void IncreaseNumberOfBusyness() { _numberOfBusynessDays += 1; }
};

bool operator ==(const Book &left, const Book &right)
{
	if (strcmp(left._ISBN, right._ISBN) == 0)
		return true;
	return false;
}

bool operator !=(const Book &left, const Book &right)
{
	return !(left == right);
}
ostream & operator << (ostream & printing, const Book & obj)
{
	if (obj._title != nullptr)
		printing << obj._title;
	if (obj._author != nullptr)
		printing << " (" << obj._author << ")" << endl;
	printing << "ISBN: " << obj._ISBN << endl;
	printing << "Genre: " << obj._genre << endl;
	printing << "Year of publishing: " << obj._yearOfPublishing << endl;
	if (obj._Member != nullptr)
		printing << "Book is reserved " << obj._numberOfBusynessDays << " day(s). Member that reserved it: " << obj._Member->GetName() << endl;
	else
		printing << "Book isn't reserved." << endl;
	return printing;
}

class Shelf
{
	int& _shelfID;
	int _capacity;
	int _numberOfBooks;
	Book* _books;

	int _tempShelfID;
public:

	Shelf(int tempShelf = 0) : _shelfID(tempShelf)
	{
		_capacity = -1;
		_numberOfBooks = 0;
		_books = nullptr;
	}
	Shelf(int shelfID, int capacity) : _tempShelfID(shelfID), _shelfID(_tempShelfID), _numberOfBooks(0), _capacity(capacity)
	{
		_books = new Book[_capacity];
	}
	~Shelf()
	{
		if (_books != nullptr)
		{
			delete[]_books;
			_books = nullptr;
		}
	}
	//Function to add a new book to the shelf
	void operator +=(const Book & Book)
	{
		if (_numberOfBooks == _capacity)
		{
			cout << "This shelf is full. Add book to the other shelf." << endl;
			return;
		}

		for (size_t i = 0; i < _numberOfBooks; i++)
			if (_books[i] == Book)
				return;

		_books[_numberOfBooks] = Book;
		_numberOfBooks += 1;
	}
	//Function to remove the book from the shelf
	void operator -=(const Book & Book)
	{
		bool ifExists = false;
		for (size_t i = 0; i < _numberOfBooks; i++)
			if (_books[i] == Book)
				ifExists = true;
		if (!ifExists)
			return;

		Book * temp = new Book[_numberOfBooks - 1];

		int k = 0;
		for (size_t i = 0; i < _numberOfBooks; i++)
			if (_books[i] != Book)
				temp[k++] = _books[i];

		if (_books != nullptr)
			delete[]_books;

		_books = temp;
		_numberOfBooks -= 1;
	}
	// Info function that prints data about all free books, all Bookmans and Bookmakers reads more
	// of n days, where n is associated as a parameter
	void Info(int n)const
	{
		int nonReservedCounter = 0;
		int ReservedCounter = 0;
		int thirdBookCounter = 0;
		for (size_t i = 0; i < _numberOfBooks; i++)
		{
			if (_books[i].CheckIfReserved())
				ReservedCounter++;
			if (!_books[i].CheckIfReserved())
				nonReservedCounter++;
			if (_books[i].CheckIfReserved() && _books[i].GetNumberOfBusyness() > n)
				thirdBookCounter++;
		}
		Book * temp = new Book[ReservedCounter];

		int k = 0;
		for (size_t i = 0; i < _numberOfBooks; i++)
			if (_books[i].CheckIfReserved())
				temp[k++] = _books[i];
		cout << "\n\t::RESERVED BOOKS::\n";
		for (size_t i = 0; i < ReservedCounter; i++)
			cout << temp[i] << endl;

		delete[] temp;

		temp = new Book[nonReservedCounter];
		k = 0;
		for (size_t i = 0; i < _numberOfBooks; i++)
			if (!_books[i].CheckIfReserved())
				temp[k++] = _books[i];
		cout << "\n\t::NOT RESERVED BOOKS::\n";
		for (size_t i = 0; i < nonReservedCounter; i++)
			cout << temp[i] << endl;

		delete[] temp;

		temp = new Book[thirdBookCounter];
		k = 0;
		for (size_t i = 0; i < _numberOfBooks; i++)
			if (_books[i].CheckIfReserved() && _books[i].GetNumberOfBusyness() > n)
				temp[k++] = _books[i];
		cout << "\n\t::BOOKS THAT ARE RESERVED MORE THAN " << n << " DAYS::\n";
		for (size_t i = 0; i < thirdBookCounter; i++)
			cout << temp[i] << endl;
	}

	int GetNumberOfBooks()const { return _numberOfBooks; }

	Book & GetBook(int index)
	{
		if (index >= 0 && index < _numberOfBooks)
			return _books[index];
	}

	Shelf & operator = (const Shelf & right)
	{
		if (this != &right)
		{
			if (_capacity != right._capacity)
			{
				_capacity = right._capacity;
				_numberOfBooks = right._numberOfBooks;

				_tempShelfID = right._tempShelfID;
				_shelfID = _tempShelfID;

				Book * temp = new Book[_capacity];

				for (size_t i = 0; i < _numberOfBooks; i++)
					temp[i] = _books[i];

				if (_books != nullptr)
					delete[]_books;

				_books = temp;
			}
			else
			{
				_numberOfBooks = right._numberOfBooks;

				_tempShelfID = right._tempShelfID;
				_shelfID = _tempShelfID;

				Book * temp = new Book[_capacity];

				for (size_t i = 0; i < _numberOfBooks; i++)
					temp[i] = _books[i];

				if (_books != nullptr)
					delete[]_books;

				_books = temp;
			}
		}
		return *this;
	}
};

class Library
{
	Shelf _booksCollection[100];
	int _shelfID;
	Member* _members; //array should be expanded by adding new member
	int _numberOfMembers;

	static int reservedBooks;
	static int searchCount;
public:

	Library()
	{
		_numberOfMembers = 0;
		_shelfID = 0;
		_members = nullptr;
	}
	~Library()
	{
		if (_members != nullptr)
		{
			delete[]_members;
			_members = nullptr;
		}
	}

	// Function to add a new member to the library
	void operator +=(const Member & Member)
	{
		for (size_t i = 0; i < _numberOfMembers; i++)
		{
			if (_members[i] == Member)
			{
				cout << "You can't add the same members." << endl;
				return;
			}
		}

		Member::ResetMmberCounter();
		Member * temp = new Member[_numberOfMembers + 1];
		for (size_t i = 0; i < _numberOfMembers; i++)
			temp[i] = _members[i];
		temp[_numberOfMembers] = Member;

		if (_members != nullptr)
			delete[]_members;

		_members = temp;
		_numberOfMembers++;
	}
	// Function to add a new shelf to the library
	void AddShelf(const Shelf & Shelf)
	{
		if (_shelfID >= 100)
			return;
		_booksCollection[_shelfID] = Shelf;
		_shelfID += 1;
	}
	// Function to add a new books to the shelf which ID is sent as parametar
	bool AddBook(const Book & Book, int shelfID)
	{
		shelfID--;
		if (shelfID < 0 || shelfID > 100)
			return false;

		_booksCollection[shelfID] += Book;
	}
	// Function to remove a books from the shelf which ID is sent as parametar
	bool RemoveBook(const Book & Book, int shelfID)
	{
		shelfID--;
		if (shelfID < 0 || shelfID > 100)
			return false;

		bool ifExists = false;
		for (size_t i = 0; i < _booksCollection[shelfID].GetNumberOfBooks(); i++)
			if (_booksCollection[shelfID].GetBook(i) == Book)
			{
				ifExists = true;
				_booksCollection[shelfID] -= Book;
			}
		if (!ifExists)
			return false;
		return true;
	}
	// Function to give a book to a member. Parameters that are sent are member number and ISBN number of the book.
	// Make sure that the member is a member of the library.
	// Make sure the book isn't already busy. A member can have a maximum of two books in charge.
	void GiveBook(int memberNumber, char * ISBN)
	{
		bool ifMemberExists = false;
		int location = 0;
		for (size_t i = 0; i < _numberOfMembers; i++)
		{
			if (_members[i] == memberNumber)
			{
				if (_members[i].GetNumberOfRentedBooks() == 2)
					return;
				ifMemberExists = true;
				location = i;
			}

		}
		if (!ifMemberExists)
			return;

		for (size_t i = 0; i < _shelfID; i++)
		{
			for (size_t j = 0; j < _booksCollection[i].GetNumberOfBooks(); j++)
			{
				if (strcmp(_booksCollection[i].GetBook(j).GetISBN(), ISBN) == 0)
				{
					if (!_booksCollection[i].GetBook(j).CheckIfReserved())
						_booksCollection[i].GetBook(j).ReserveBook(_members[location]);
				}
			}
		}

	}
	// Function that releases a book with ISBN number that is sent as parametar
	void ReleaseBook(char * ISBN)
	{
		for (size_t i = 0; i < _shelfID; i++)
		{
			for (size_t j = 0; j < _booksCollection[i].GetNumberOfBooks(); j++)
			{
				if (strcmp(_booksCollection[i].GetBook(j).GetISBN(), ISBN) == 0)
				{
					if (_booksCollection[i].GetBook(j).CheckIfReserved())
						_booksCollection[i].GetBook(j).ReleaseBook();
				}
			}
		}
	}
	//Function that increases number of days of busyness of all books for 1
	void IncreaseNumberOfBusyness()
	{
		for (size_t i = 0; i < _shelfID; i++)
		{
			for (size_t j = 0; j < _booksCollection[i].GetNumberOfBooks(); j++)
				if (_booksCollection[i].GetBook(j).CheckIfReserved())
					_booksCollection[i].GetBook(j).IncreaseNumberOfBusyness();
		}
	}
	//Function that finds and prints all non reserved books on the shelf (shelfID sent as parametar)
	void printingOfAllNonReservedBooksOnTheShelf(int shelfID)
	{
		shelfID--;
		if (shelfID < 0 || shelfID > 100)
			return;
		int counter = 0;
		for (size_t i = 0; i < _booksCollection[shelfID].GetNumberOfBooks(); i++)
			if (!_booksCollection[shelfID].GetBook(i).CheckIfReserved())
				counter++;
		Book * temp = new Book[counter];

		int k = 0;
		for (size_t i = 0; i < _booksCollection[shelfID].GetNumberOfBooks(); i++)
			if (!_booksCollection[shelfID].GetBook(i).CheckIfReserved())
				temp[k++] = _booksCollection[shelfID].GetBook(i);

		cout << "\n\t::NON RESERVED BOOKS ON THE SHELF NUMBER " << shelfID + 1 << "::\n";
		for (size_t i = 0; i < counter; i++)
			cout << temp[i] << endl;

		if (counter == 0)
			cout << "\nThere is no non reserved books on this shelf!" << endl;
	}
	// Function that finds the book by title and / or author and prints all data as well as whether book is currently in charge
	Book * FindBooks(char * author, char * title)
	{
		for (size_t i = 0; i < _shelfID; i++)
		{
			for (size_t j = 0; j < _booksCollection[i].GetNumberOfBooks(); j++)
				if (strcmp(_booksCollection[i].GetBook(j).GetAuthor(), author) == 0 || strcmp(_booksCollection[i].GetBook(j).GetTitle(), title) == 0)
					searchCount++;
		}

		Book * temp = new Book[searchCount];

		int k = 0;
		for (size_t i = 0; i < _shelfID; i++)
		{
			for (size_t j = 0; j < _booksCollection[i].GetNumberOfBooks(); j++)
				if (strcmp(_booksCollection[i].GetBook(j).GetAuthor(), author) == 0 || strcmp(_booksCollection[i].GetBook(j).GetTitle(), title) == 0)
					temp[k++] = _booksCollection[i].GetBook(j);
		}

		return temp;
	}
	//Function that finds all the books that have been in charge for more than 14 days and prints the information about the members reserved those books
	Book * FindReservedBooks()
	{
		for (size_t i = 0; i < _shelfID; i++)
		{
			for (size_t j = 0; j < _booksCollection[i].GetNumberOfBooks(); j++)
			{
				if (_booksCollection[i].GetBook(j).GetNumberOfBusyness() > 14)
					reservedBooks++;
			}
		}

		Book * temp = new Book[reservedBooks];

		int k = 0;

		for (size_t i = 0; i < _shelfID; i++)
		{
			for (size_t j = 0; j < _booksCollection[i].GetNumberOfBooks(); j++)
			{
				if (_booksCollection[i].GetBook(j).GetNumberOfBusyness() > 14)
					temp[k++] = _booksCollection[i].GetBook(j);
			}
		}

		return temp;
	}

	static int GetNumberOfReservedBooks() { return reservedBooks; }
	static int getsearchCount() { return searchCount; }
};

int Library::reservedBooks = 0;
int Library::searchCount = 0;

const char * crt = "\n-----------------------------------------------------\n";

void main()
{
	// Test all available library functionality of the Library
	// Add at least 5 Shelfs per 5 Book to a library class object, and join at least 10 members

#pragma region Members
	Member nisvet("Nisvet Mujkic", "060/302-5434");
	Member seherzada("Seherzada Mujkic", "060/333-4444");
	Member eman("Eman Basic", "060/371-4648");
	Member mirnes("Mirnes Cerimovic", "060/212-3399");
	Member eldin("Eldin Husic", "060/214-8821");
	Member adnan("Adnan Murtic", "060/654-6434");
	Member elvedin("Elvedin Dzihanovic", "060/987-3283");
	Member amir("Amir Besirevic", "060/313-9926");
	Member haris("Haris Poljak", "060/715-7661");
	Member almin("Almin Handanovic", "060/654-1192");
#pragma endregion

#pragma region Books
	Book dervis("974-933-220-545-9", "Dervis i smrt", "Mesa Selimovic", "Psiholoski", 1977);
	Book crveno("978-953-021-442-1", "Crveno i crno", "Stendhal", "Psiholoski", 1988);
	Book ana("978-995-278-289-9", "Ana Karenjina", "Lav Tolstoj", "Historijski", 1833);
	Book vuk("978-994-253-068-3", "Stepski vuk", " Hermann Hesse", "Patriotski", 1962);
	Book avlija("995-666-154-222-4", "Prokleta avlija", "Ivo Andric", "Kriminalisticki", 1981);
	Book crvenkapa("978-863-521-227-2", "Sve crvenkape su iste", "Marko Vidojković", "Psiholoski", 2001);
	Book oliver("978-816-521-224-9", "Oliver VII", "Antal Serb", "Ratni", 1971);
	Book kandid("995-717-215-432-5", "Candide Zadig", "Voltaire", "Ljubavni", 1833);
	Book tvrdjava("998-121-101-961-7", "Tvrdjava", "Mesa Selimovic", "Ljubavni", 1954);
	Book inferno("978-995-830-177-3", "Inferno", "Dan Brown", "Razbojnicki", 2005);
#pragma endregion

#pragma region Shelfs
	Shelf first(1, 5), second(2, 5), third(3, 5), fourth(4, 5), fifth(5, 5);
#pragma endregion

	Library mostar;

#pragma region AddingShelfBookAndMembers
	mostar.AddShelf(first);
	mostar.AddShelf(second);
	mostar.AddShelf(third);
	mostar.AddShelf(fourth);
	mostar.AddShelf(fifth);

	mostar.AddBook(dervis, 1);
	mostar.AddBook(crveno, 1);

	mostar.AddBook(ana, 2);
	mostar.AddBook(vuk, 2);

	mostar.AddBook(avlija, 3);
	mostar.AddBook(crvenkapa, 3);

	mostar.AddBook(oliver, 4);
	mostar.AddBook(kandid, 4);

	mostar.AddBook(tvrdjava, 5);
	mostar.AddBook(inferno, 5);

	mostar += nisvet;
	mostar += seherzada;
	mostar += eman;
	mostar += mirnes;
	mostar += eldin;
#pragma endregion

	mostar.GiveBook(1, "974-933-220-545-9");
	mostar.GiveBook(1, "978-953-021-442-1");
	mostar.GiveBook(1, "978-816-521-224-9"); // can not reserve a book because it is allowed up to two Books per member

	mostar.ReleaseBook("978-953-021-442-1");

	mostar.printingOfAllNonReservedBooksOnTheShelf(1); // should only print "Crveno i crno" because it was previously released

	mostar.RemoveBook(crveno, 1);

	cout << crt;
	mostar.printingOfAllNonReservedBooksOnTheShelf(1); // printing should be blank because book "Crveno i crno" is removed
	cout << crt;

	Book * ptrNameTitle = mostar.FindBooks("Mesa Selimovic", "Prokleta avlija"); // should print two books from "Mesa Selimovic" and one from "Ivo Andric"

	cout << crt << "\tBooks that are found by title or author" << crt;
	for (size_t i = 0; i < Library::getsearchCount(); i++)
		cout << ptrNameTitle[i] << endl;
	cout << crt;

	for (size_t i = 0; i < 20; i++)
		mostar.IncreaseNumberOfBusyness();

	cout << crt << "\tBooks that are reserved more than 14 days" << crt;
	Book * BooksReservedForLongerPeriod = mostar.FindReservedBooks();

	for (size_t i = 0; i < Library::GetNumberOfReservedBooks(); i++)
		cout << BooksReservedForLongerPeriod[i] << endl;
	cout << crt;

	_getch();
}