#include<iostream>
using namespace std;
char *crt = "\n---------------------------\n";

class Exam
{
protected:
	char* _course;
	char _examDate[12]; 
	char _examTime[6];
	int _duration; //expressed in minutes
	int _maxPoints; //max number of points is changing when we add questions in collection
	double _result; //percentage result of exama
public:
	Exam() {
		_course = nullptr;
		_maxPoints = 0;
		_result = 0;
		_duration = 0;
	}
	Exam(char* course, char* examDate, char* examTime, int duration, int maxPoints = 0)
	{
		int size = strlen(course) + 1;
		_course = new char[size];
		strcpy_s(_course, size, course);

		strncpy_s(_examDate, examDate, _TRUNCATE);
		strncpy_s(_examTime, examTime, _TRUNCATE);

		_duration = duration;
		_maxPoints = maxPoints;
		_result = 0;
	}
	Exam(Exam &original) {
		int size = strlen(original._course) + 1;
		_course = new char[size];
		strcpy_s(_course, size, original._course);

		strncpy_s(_examDate, original._examDate, _TRUNCATE);
		strncpy_s(_examTime, original._examTime, _TRUNCATE);

		_duration = original._duration;
		_maxPoints = original._maxPoints;
		_result = original._result;
	}
	Exam& operator=(Exam &original) {
		if (this == &original)
			return *this;

		delete[]_course;
		int size = strlen(original._course) + 1;
		_course = new char[size];
		strcpy_s(_course, size, original._course);

		strncpy_s(_examDate, original._examDate, _TRUNCATE);
		strncpy_s(_examTime, original._examTime, _TRUNCATE);

		_duration = original._duration;
		_maxPoints = original._maxPoints;
		_result = original._result;
		return *this;
	}
	friend ostream& operator<<(ostream& COUT, Exam &original);

	~Exam()
	{
		delete[] _course;
		_course = nullptr;
	}

};
ostream& operator<<(ostream& COUT, Exam &original) {
	COUT << original._course << ": Starts: " << original._examTime << "  Lasts: " << original._duration << "min" << endl;
	COUT << "Max points: " << original._maxPoints << endl;

	return COUT;
}

template <class T1, class T2, int max>
class Collection
{
	T1 _elements1[max];
	T2 _elements2[max];
	int _currently;
public:
	Collection() {
		_currently = 0;
	}
	bool AddElement(T1 element1, T2 element2) {
		if (_currently == max)
			return false;

		_elements1[_currently] = element1;
		_elements2[_currently] = element2;
		_currently++;

		return true;
	}
	Collection& operator-=(int location) {
		if (location < 0 || location > _currently)
			return *this;

		for (int i = location - 1; i < _currently - 1; i++)
		{
			_elements1[i] = _elements1[i + 1];
			_elements2[i] = _elements2[i + 1];
		}
		_currently--;
		return *this;
	}

	T1& getElement1(int location) {
		if (location >= 1 && location <= _currently)
			return _elements1[location - 1];
	}
	T2& getElement2(int location) {
		if (location >= 1 && location <= _currently)
			return _elements2[location - 1];
	}
	T1& operator[](int location) // returns element1 (first data type that is sent)
	{
		if (location >= 1 && location <= _currently)
			return _elements1[location - 1];
	}
	int getCurrently() { return _currently; }
};

class WrittenExam : public Exam
{
protected:
	Collection<char*, int, 50>* _questions; //Collection of questions with points, element1 = question, element2 = numberOfPossiblePointsForQuestion
	Collection<char*, int, 50> _answers; //Collection of answers with gained points, element1 = answer, element2 = gainedPoints 
	int _numberOfRightAnswers;
	int _gainedPoints;
public:
	//Constructor and destructor
	WrittenExam() :Exam() {
		_questions = new Collection<char*, int, 50>;
		_numberOfRightAnswers = 0;
		_gainedPoints = 0;
	}
	WrittenExam(char* course, char* examDate, char* examTime, int duration, int maxPoints = 0, int numberOfRightAnswers = 0, int gainedPoints = 0)
		:Exam(course, examDate, examTime, duration, maxPoints)
	{
		_questions = new Collection<char*, int, 50>;
		_numberOfRightAnswers = numberOfRightAnswers;
		_gainedPoints = gainedPoints;
	}
	//Copy constructor
	WrittenExam(WrittenExam &original) : Exam(original) {
		_questions = new Collection<char*, int, 50>(*original._questions);
		_answers = original._answers;
		_numberOfRightAnswers = original._numberOfRightAnswers;
		_gainedPoints = original._gainedPoints;
	}
	WrittenExam& operator=(WrittenExam &original) {
		if (this == &original)
			return *this;

		Exam::operator=(Exam(original));

		/*delete[]_course;
		int size = strlen(original._course) + 1;
		_course = new char[size];
		strcpy_s(_course, size, original._course);

		strncpy_s(_examDate, original._examDate, _TRUNCATE);
		strncpy_s(_examTime, original._examTime, _TRUNCATE);

		_duration = original._duration;
		_maxPoints = original._maxPoints;
		_result = original._result;
		*/

		_questions = new Collection<char*, int, 50>(*original._questions);
		_numberOfRightAnswers = original._numberOfRightAnswers;
		_gainedPoints = original._gainedPoints;
		return *this;
	}
	WrittenExam& operator()(char *question, int points) {
		if (points < 0)
			return *this;

		for (int i = 0; i < _questions->getCurrently(); i++) {
			if (strcmp(question, _questions->getElement1(i + 1)) == 0)
				return *this;
		}

		char *temp = new char[strlen(question) + 1]; 
		strcpy_s(temp, strlen(question) + 1, question);

		_questions->AddElement(temp, points);
		_maxPoints += points;
		return *this;
	}

	//Operator() to add a question to the collection with the corresponding number of points.
	//Exam can not contain the same questions. The number of points per question can not be negative.

	//Operator + = to add a response to a question. Answers are added in the order of adding a question.
	//The number of points won initially is set to 0. It is possible to add blank text as an answer.

	WrittenExam& operator+=(char *answer) {
		_answers.AddElement(answer, 0);
		return *this;
	}

	//Function to evaluate the right answer, ordinal number of the question from collection and the right answer are sent as parameters
	bool evaluateQuestion(int questionOrdinalNumber, char* rightAnswer) {
		if (strcmp(_answers.getElement1(questionOrdinalNumber), rightAnswer) == 0)
		{
			_numberOfRightAnswers++;
			_gainedPoints += _questions->getElement2(questionOrdinalNumber);
			_answers.getElement2(questionOrdinalNumber) = _questions->getElement2(questionOrdinalNumber); 
																							   
																							   
			_result = (double(_gainedPoints) / _maxPoints) * 100;

			return true;
		}
		return false;
	}

	~WrittenExam() {
		for (int i = 0; i < _questions->getCurrently(); i++) //in operator(), new memory has been alocated for this
			delete[] _questions->getElement1(i + 1);

		delete _questions; // only one collection of questions is deleted, not the array
		_questions = nullptr;
	}
	friend ostream& operator<<(ostream& COUT, WrittenExam &original);
};
ostream& operator<<(ostream& COUT, WrittenExam &original) {
	cout << Exam(original) << endl;
	COUT << "Number of right answers " << original._numberOfRightAnswers << endl;
	COUT << "Gained points " << original._gainedPoints << endl;
	COUT << "Result: " << original._result << "%" << endl;
	return COUT;
}


class PracticalExam :public Exam
{
protected:
	Collection<char*, char*, 3> _tools; //required tools and their versions, eg "Visual Studio" tool, version "2013"
	char* _task; //task text
	bool _isPassed; //true if the student passed exam, otherwise false
public:
	//Based on the previous implementation of the class WrittenExam design and implement
	//necessary functions for working with the PracticalExam class.

	PracticalExam(char *task = "---") :Exam()
	{
		int size = strlen(task) + 1;
		_task = new char[size];
		strcpy_s(_task, size, task);
		_isPassed = false;
	}
	PracticalExam(char* course, char* examDate, char* examTime, int duration, int maxPoints, char *task)
		:Exam(course, examDate, examTime, duration, maxPoints)
	{
		int size = strlen(task) + 1;
		_task = new char[size];
		strcpy_s(_task, size, task);
		_isPassed = false;
	}
	~PracticalExam()
	{
		for (int i = 0; i < _tools.getCurrently(); i++) { //in operator(), new memory has been alocated for this
			delete[]_tools.getElement1(i + 1);
			delete[]_tools.getElement2(i + 1);
		}
		delete[]_task;
		_task = nullptr;
	}
	PracticalExam(PracticalExam& original) : Exam(original)
	{
		_tools = original._tools;
		int size = strlen(original._task) + 1;
		_task = new char[size];
		strcpy_s(_task, size, original._task);
		_isPassed = original._isPassed;
	}
	PracticalExam& operator=(PracticalExam& original)
	{
		if (this != &original) {

			Exam::operator=(Exam(original));

			/*
			delete[]_course;
			int size = strlen(original._course) + 1;
			_course = new char[size];
			strcpy_s(_course, size, original._course);

			strncpy_s(_examDate, original._examDate, _TRUNCATE);
			strncpy_s(_examTime, original._examTime, _TRUNCATE);

			_duration = original._duration;
			_maxPoints = original._maxPoints;
			_result = original._result;
			*/

			delete[]_task;
			_tools = original._tools;
			int size = strlen(original._task) + 1;
			_task = new char[size];
			strcpy_s(_task, size, original._task);
			_isPassed = original._isPassed;
		}
		return *this;
	}
	PracticalExam& operator()(char *tool, char* version) {
		for (int i = 0; i < _tools.getCurrently(); i++) {
			if (strcmp(tool, _tools.getElement1(i + 1)) == 0 && strcmp(version, _tools.getElement2(i + 1)) == 0)
				return *this;
		}

		char *temp = new char[strlen(tool) + 1]; 
		strcpy_s(temp, strlen(tool) + 1, tool);

		char *temp2 = new char[strlen(version) + 1];
		strcpy_s(temp2, strlen(version) + 1, version);

		_tools.AddElement(temp, temp2);
		return *this;
	}
	void evaluateTest(char* value) {
		cout << "Has student: " << value << " passed practical exam? (y / n)" << endl;
		cout << "Used tools: " << endl;
		for (int i = 0; i < _tools.getCurrently(); i++)
			cout << _tools.getElement1(i + 1) << "  " << _tools.getElement2(i + 1) << endl;

		char odg = 'n';
		cout << "Answer: ";
		cin >> odg;
		if (odg == 'Y' || odg == 'y')
			_isPassed = true;
		else
			_isPassed = false;

	}
	friend ostream& operator<<(ostream& COUT, PracticalExam &original);

};
ostream& operator<<(ostream& COUT, PracticalExam &original) {
	cout << Exam(original) << endl;
	cout << "Task: " << original._task << endl;
	cout << "Passed: " << boolalpha << original._isPassed << endl;

	return COUT;
}


int main()
{
	// Test all available functionality through the given collection.
	// The second parameter of the collection is the student ID
	char *studenti[] = { "IB150219","IB150229","IB150239","IB150249","IB150259","IB150269" };

	Collection<WrittenExam, char*, 100> _WrittenExams;
	WrittenExam smaP("SMA", "12.1.2016", "12:00", 30);

	_WrittenExams.AddElement(smaP, studenti[0]);
	_WrittenExams.AddElement(smaP, studenti[1]);
	_WrittenExams.AddElement(smaP, studenti[2]);

	Collection<PracticalExam, char*, 100> _PracticalExams;
	PracticalExam smaR("SMA", "17.1.2016", "13:00", 120, 100, "Send an encrypted mail with mentioned tools");

	_PracticalExams.AddElement(smaR, studenti[3]);
	_PracticalExams.AddElement(smaR, studenti[4]);
	_PracticalExams.AddElement(smaR, studenti[5]);
	_PracticalExams.AddElement(smaR, studenti[0]);

#pragma region TestiranjePismeno
#pragma region Student1
	_WrittenExams[1]("What is port for telnet", 10);//pitanja na testu iz sma za studenta 1  (IB150219)
	_WrittenExams[1] += "23";						//Njegov answer na question
	_WrittenExams[1]("What is port for SSH", 10);
	_WrittenExams[1] += "22";						//Njegov answer na question	
	_WrittenExams[1]("What is port for DNS", 10);
	_WrittenExams[1] += "53";
	_WrittenExams[1]("What is port for HTTPS", 10);
	_WrittenExams[1] += "443";
	_WrittenExams[1]("What is port for HTTP", 10);
	_WrittenExams[1] += "80";
	_WrittenExams[1]("What are ports for DHCP", 20);
	_WrittenExams[1] += "67 68";
	_WrittenExams[1]("What are ports for FTP", 20);
	_WrittenExams[1] += "20 21";

	_WrittenExams[1].evaluateQuestion(1, "23");//(ordinal number of the question, right answer(s) to that question)
	_WrittenExams[1].evaluateQuestion(2, "22");	 
	_WrittenExams[1].evaluateQuestion(3, "53");
	_WrittenExams[1].evaluateQuestion(4, "443");
	_WrittenExams[1].evaluateQuestion(5, "80");
	_WrittenExams[1].evaluateQuestion(6, "67 68");
	_WrittenExams[1].evaluateQuestion(7, "20 21");
#pragma endregion
#pragma region Student2
	_WrittenExams[2]("What is port for telnet", 10);//questions for student2 (IB150229) 
	_WrittenExams[2] += "23";						//His/her answer to that question
	_WrittenExams[2]("What is port for SSH", 10);
	_WrittenExams[2] += "22";						//His/her answer to that question
	_WrittenExams[2]("What is port for DNS", 10);
	_WrittenExams[2] += "53";
	_WrittenExams[2]("What is port for HTTPS", 10);
	_WrittenExams[2] += "999"; // wrong
	_WrittenExams[2]("What is port for HTTP", 10);
	_WrittenExams[2] += "999"; // wrong
	_WrittenExams[2]("What are ports for za DHCP (write with a space, without a comma)", 20);
	_WrittenExams[2] += "67 68";
	_WrittenExams[2]("What are ports for FTP (write with a space, without a comma)", 20);
	_WrittenExams[2] += "20 21";

	_WrittenExams[2].evaluateQuestion(1, "23");
	_WrittenExams[2].evaluateQuestion(2, "22");
	_WrittenExams[2].evaluateQuestion(3, "53");
	_WrittenExams[2].evaluateQuestion(4, "443");
	_WrittenExams[2].evaluateQuestion(5, "80");
	_WrittenExams[2].evaluateQuestion(6, "67 68");
	_WrittenExams[2].evaluateQuestion(7, "20 21");
#pragma endregion
#pragma region Student3
	_WrittenExams[3]("What is port for telnet", 10);//questions for student3 (IB150239)  
	_WrittenExams[3] += "23";						//His/her answer to that question
	_WrittenExams[3]("What is port for SSH", 10);
	_WrittenExams[3] += "22";						//His/her answer to that question
	_WrittenExams[3]("What is port for DNS", 10);
	_WrittenExams[3] += "53";
	_WrittenExams[3]("What is port for HTTPS", 10);
	_WrittenExams[3] += "999"; // wrong
	_WrittenExams[3]("What is port for HTTP", 10);
	_WrittenExams[3] += "999"; // wrong
	_WrittenExams[3]("What are ports for DHCP", 10);
	_WrittenExams[3] += "67 68";
	_WrittenExams[3]("What are ports for FTP", 10);
	_WrittenExams[3] += "20 21";
	_WrittenExams[3]("What is port for SMTP", 20);
	_WrittenExams[3] += "25";
	_WrittenExams[3]("What are ports for port for SNMP", 20);
	_WrittenExams[3] += "161 162";

	_WrittenExams[3].evaluateQuestion(1, "23");			//right answers
	_WrittenExams[3].evaluateQuestion(2, "22");			
	_WrittenExams[3].evaluateQuestion(3, "53");			
	_WrittenExams[3].evaluateQuestion(4, "443");		
	_WrittenExams[3].evaluateQuestion(5, "80");
	_WrittenExams[3].evaluateQuestion(6, "67 68");
	_WrittenExams[3].evaluateQuestion(7, "20 21");
	_WrittenExams[3].evaluateQuestion(8, "25");
	_WrittenExams[3].evaluateQuestion(9, "161 162");
#pragma endregion
#pragma region WrittenExamResults
	cout << crt << "Results of written exams : " << crt << crt;
	for (int i = 0; i < 3; i++)
	{
		cout << _WrittenExams.getElement2(i + 1) << crt; 
		cout << _WrittenExams[i + 1] << crt; 

	}
#pragma endregion
#pragma endregion	

#pragma region TestiranjePrakticno


	//Student 4
	_PracticalExams[1]("gpg4win", "2.3.3");
	_PracticalExams[1]("Outlook Desktop App", "2016");
	system("color 0f");


	//Student 5
	_PracticalExams[2]("K-9 Mail", "Google Play Latest");
	_PracticalExams[2]("OpenKeyChain", "Google Play Latest");
	system("color f0");

	//Student 6
	_PracticalExams[3]("K-9 Mail", "Google Play Latest");
	_PracticalExams[3]("OpenKeyChain", "Google Play Latest");
	system("color 0f");


	//Student 1
	_PracticalExams[4]("K-9 Mail", "Google Play Latest");
	_PracticalExams[4]("PGP", "Google Play Latest");
	system("color f0");

	cout << crt << "Evaluate of practical exams: " << crt;
	for (int i = 0; i < _PracticalExams.getCurrently(); i++)
	{
		system("color 0f");
		_PracticalExams[i + 1].evaluateTest(_PracticalExams.getElement2(i + 1));
		system("color f0");
		cout << crt;
	}

	cout << crt << "Results of practical exams: " << crt;
	for (int i = 0; i < _PracticalExams.getCurrently(); i++)
	{
		cout << _PracticalExams[i + 1] << crt;
	}
#pragma endregion


	return 0;
}