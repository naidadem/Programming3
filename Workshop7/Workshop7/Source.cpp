#include<iostream>
using namespace std;

char* TypeInString(char* _string) {
	if (_string == nullptr)
		return nullptr;
	int size = strlen(_string) + 1;
	char* temp = new char[size];
	strcpy_s(temp, size, _string);
	return temp;
}

template<class T1, class T2, int max>
class FITCollection {
	int* _currently;
	T1 _elements1[max];
	T2* _elements2[max];
public:
	FITCollection();

	FITCollection(FITCollection<T1, T2, max>&);

	~FITCollection();

	FITCollection& operator = (FITCollection<T1, T2, max>&);

	void operator()(T1, T2);

	T1& operator[](int);

	void operator-=(int);

	friend ostream& operator << <>(ostream&, FITCollection<T1, T2, max>&);
};

//default constructor
template<class T1, class T2, int max>
FITCollection<T1, T2, max>::FITCollection() {
	_currently = new int(0);
	for (int i = 0; i < max; i++)
		_elements2[i] = nullptr;
}

//copy constructor
template<class T1, class T2, int max>
FITCollection<T1, T2, max>::FITCollection(FITCollection<T1, T2, max>& a) {
	_currently = new int(*a._currently);
	for (int i = 0; i < *a._currently; i++) {
		_elements1[i] = a._elements1[i];
		_elements2[i] = new T2(*a._elements2[i]);
	}
}

//destructor
template<class T1, class T2, int max>
FITCollection<T1, T2, max>::~FITCollection() {
	for (int i = 0; i < *_currently; i++) {
		delete _elements2[i];
		_elements2[i] = nullptr;
	}
	delete _currently;
	_currently = nullptr;
}

//basic assignment operator
template<class T1, class T2, int max>
FITCollection<T1, T2, max>& FITCollection<T1, T2, max>::operator = (FITCollection<T1, T2, max>& a) {
	if (this == &a)
		return *this;
	delete _currently;
	_currently = new int(*a._currently);

	for (int i = 0; i < *_currently; i++)
		delete _elements2[i];

	for (int i = 0; i < *_currently; i++) {
		_elements1[i] = a._elements1[i];
		_elements2[i] = new T2(*a._elements2[i]);
	}
	return *this;
}

//operator() allows adding new elements to the collection
template<class T1, class T2, int max>
void FITCollection<T1, T2, max>::operator()(T1 t1, T2 t2) {
	if (*_currently >= max)
		return;
	_elements1[*_currently] = t1;
	_elements2[*_currently] = new T2(t2);
	(*_currently) += 1;
}

//operator [] returs T1 type element based on its location in the array
template<class T1, class T2, int max>
T1& FITCollection<T1, T2, max>::operator[](int location) {
	if (location >= *_currently || location < 0)
		throw exception("Greska. location nije validna!");
	return _elements1[location];//mozda je greska
}

//operator -= removes elements from the collection based on ordinal number sent as parameter              
template<class T1, class T2, int max>
void FITCollection<T1, T2, max>::operator-=(int location) {
	if (location >= *_currently || location < 0)
		throw exception("Error. Location isn't valid!");
	delete _elements2[location];
	_elements2[location] = nullptr;
	for (int i = location + 1; i <*_currently; i++)
	{
		_elements1[i - 1] = _elements1[i];
		_elements2[i - 1] = _elements2[i];
	}
	(*_currently)--;
}

//operator << friend ostream& operator << <> (ostream&, FITCollection<T1, T2, max>&)
template<class T1, class T2, int max>
ostream& operator<< <>(ostream& c, FITCollection<T1, T2, max>& a) {
	for (int i = 0; i < *a._currently; i++)
		c << *a._elements2[i] << "\t" << a._elements1[i] << endl;
	return c;
}

//=============================================================================
class Questionnaire {
	char* _question;
	int _duration; //expressed in days
				   //collection needs to store possible answers on questions with their ordinal number
	FITCollection<char*, int, 8> _answers;//FITCollection type object
public:
	Questionnaire();
	Questionnaire(Questionnaire&);
	~Questionnaire();
	Questionnaire& operator = (Questionnaire&);
	void AddTheAnswer(char*, int);
	void RemoveAnswer(int);
	void operator++(int);
	void operator++();
	void SetQuestion(char* question) {
		_question = TypeInString(question);
	}
	friend ostream& operator << (ostream&, Questionnaire&);
};

//dafault constructor
Questionnaire::Questionnaire() : _answers() {
	_question = nullptr;
	_duration = 0;
}

//copy constructor
Questionnaire::Questionnaire(Questionnaire& a) {
	_question = TypeInString(a._question);
	_duration = a._duration;
	_answers = a._answers;//basic assignment operator for FITCollection
}

//destructor
Questionnaire::~Questionnaire() {
	delete[]_question;
	_question = nullptr;
}

//operator = 
Questionnaire& Questionnaire::operator = (Questionnaire& a) {
	if (this == &a)
		return *this;
	delete[]_question;
	_question = TypeInString(a._question);
	_duration = a._duration;
	_answers = a._answers;//basic assignment operator for FITCollection
	return *this;
}

//add the answer for the questionnaire
void Questionnaire::AddTheAnswer(char* newAnswer, int ordinalNumber) {
	_answers(newAnswer, ordinalNumber);
}

//remove the answer based on its location (ordinal number)
void Questionnaire::RemoveAnswer(int location) {
	try {
		_answers -= location;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

//operator ++ post increment, increses duration of the questionnaire for one day
void Questionnaire::operator++(int) {
	_duration++;
}

//operator ++ pre increment, increses duration of the questionnaire for one day
void Questionnaire::operator++() {
	_duration++;
}

//Bitwise left shift operator
ostream& operator << (ostream& c, Questionnaire& a) {
	c << "Duration: " << a._duration << endl;
	c << "Question: " << a._question << endl;
	c << "Answers: " << endl;
	c << a._answers;
	return c;
}


int main() {
	Questionnaire a;

	a.SetQuestion("question 1");

	a.AddTheAnswer(TypeInString("first answer"), 0);
	a.AddTheAnswer(TypeInString("second answer"), 1);
	a.AddTheAnswer(TypeInString("third answer"), 2);
	a.AddTheAnswer(TypeInString("fourth answer"), 3);

	cout << a << endl;

	return 0;
}