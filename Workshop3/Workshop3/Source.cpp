#include<iostream>
using namespace std;
char *crt = "\n\--------------------------------------------------\n";

class Employee {
	int _employeeID;
	char _name[30];
	char _surname[30];
	char* _jobPosition;
public:

	//Implement constructor and destructor functions
	Employee(int employeeID = -1, char* name = "---", char* surname = "---", char* jobPosition = "---")
	{
		_employeeID = employeeID;
		strncpy_s(_name, name, _TRUNCATE);
		strncpy_s(_surname, surname, _TRUNCATE);
		int size = strlen(jobPosition) + 1;
		_jobPosition = new	char[size];
		strcpy_s(_jobPosition, size, jobPosition);
	}
	Employee(const Employee & u)
	{
		_employeeID = u._employeeID;
		strncpy_s(_name, u._name, _TRUNCATE);
		strncpy_s(_surname, u._surname, _TRUNCATE);
		int size = strlen(u._jobPosition) + 1;
		_jobPosition = new	char[size];
		strcpy_s(_jobPosition, size, u._jobPosition);
	}
	~Employee()
	{
		delete[] _jobPosition;
		_jobPosition = nullptr;
	}

	//Info function
	void Info()
	{
		cout << "Employee ID: " << _employeeID << endl;
		cout << "Name and surname: " << _name << " " << _surname << endl;
		cout << "Job position: " << _jobPosition << endl;
	}
	int getemployeeID() const
	{
		return _employeeID;
	}
	char* getname()
	{
		return _name;
	}
};


class Activity {
	char* _title;
	bool isFinished;
	int* _duration; //expressed in days
	Employee* _employees[5];
public:
	//Implement constructor and destructor functions
	Activity()
	{
		_title = nullptr;
		isFinished = false;
		_duration = new int;
		for (int i = 0; i < 5; i++)
			_employees[i] = nullptr;
	}
	Activity(char *title, int duration = 0)
	{
		int size = strlen(title) + 1;
		_title = new char[size];
		strcpy_s(_title, size, title);
		isFinished = false;
		_duration = new int(duration);
		for (int i = 0; i < 5; i++)
			_employees[i] = nullptr;
	}
	Activity(const Activity & a)
	{
		int size = strlen(a._title) + 1;
		_title = new char[size];
		strcpy_s(_title, size, a._title);
		isFinished = a.isFinished;
		_duration = new int(*a._duration);
		for (int i = 0; i < 5; i++)
		{
			if (a._employees[i] != nullptr)
				_employees[i] = new Employee(*a._employees[i]);
			else
				_employees[i] = nullptr;
		}
	}
	~Activity()
	{
		delete[] _title;
		_title = nullptr;
		for (int i = 0; i < 5; i++)
		{
			if (_employees[i] != nullptr)
			{
				delete _employees[i];
				_employees[i] = nullptr;
			}
		}
		delete _duration;
		_duration = nullptr;
	}

	// Function that allows employee to engage in project activity. The function should return the value of the type
	// bool for successful, or unsuccessful adding employee. It is not possible to add the same employee multiple times. Not possible to
	// employ employees on already completed activity.
	bool AddEmployee(const Employee & u)
	{
		//Activity finished
		if (isFinished)
			return false;

		//employee already exists
		for (int i = 0; i < 5; i++)
			if (_employees[i] != nullptr && _employees[i]->getemployeeID() == u.getemployeeID())
				return false;

		for (int i = 0; i < 5; i++)
			if (_employees[i] == nullptr)
			{
				_employees[i] = new Employee(u);
				return true;	//employee added
			}
		return false; //no place for new employee
	}

	//Function that removes employee from the activity with employee ID send as parameter
	void RemoveEmployee(int employeeID)
	{
		for (int i = 0; i < 5; i++)
			if (_employees[i] != nullptr && _employees[i]->getemployeeID() == employeeID)
			{
				delete _employees[i];
				_employees[i] = nullptr;
			}
	}

	// Function that removes all employees engaged on the activity
	void RemoveAllEmployees()
	{
		for (int i = 0; i < 5; i++)
			if (_employees[i] != nullptr)
			{
				delete _employees[i];
				_employees[i] = nullptr;
			}
	}

	//Added funtion
	void Info()
	{
		cout << "\nActivity:\n";
		cout << "Title: " << _title << endl;
		cout << "Finished: " << boolalpha << isFinished << endl;
		cout << "Duration: " << *_duration << " days\n";
		cout << "Employees: ";
		for (int i = 0; i < 5; i++)
			if (_employees[i] != nullptr)
				cout << _employees[i]->getname() << ", ";
		cout << endl;

	}
	char* gettitle() const
	{
		return _title;
	}
	Employee** getEmployees()
	{
		return _employees;
	}
	bool IsFinished()
	{
		return isFinished;
	}
	void Copy(const Activity & a)
	{
		int size = strlen(a._title) + 1;
		_title = new char[size];
		strcpy_s(_title, size, a._title);
		isFinished = a.isFinished;
		_duration = new int(*a._duration);
		for (int i = 0; i < 5; i++)
		{
			if (a._employees[i] != nullptr)
				_employees[i] = new Employee(*a._employees[i]);
			else
				_employees[i] = nullptr;
		}
	}
	void FinishActivity()
	{
		isFinished = true;
	}
	int getduration() { return *_duration; }
	void setduration(int t) { *_duration = t; }

};
int _numberOfProjects = 0;

class Project {
	char* _title;
	char* _section;
	int _duration; //expressed in months
	Activity* _activities;
	int _numberOfActivities;
public:

	//Implement constructor and destructor functions
	Project(char* title, char* section, int duration = 0)
	{
		int size = strlen(title) + 1;
		_title = new char[size];
		strcpy_s(_title, size, title);
		size = strlen(section) + 1;
		_section = new char[size];
		strcpy_s(_section, size, section);
		_duration = duration;
		_activities = nullptr;
		_numberOfActivities = 0;
		_numberOfProjects++;
	}
	~Project()
	{
		delete[] _title;
		_title = nullptr;
		delete[] _section;
		_section = nullptr;
		delete[] _activities;
		_activities = nullptr;
		_numberOfProjects--;
	}

	// Function for adding project activities. Prevent adding multiple activityes with the same title.
	// For the purpose of comparing activities, create a function within the Activity class.
	void AddActivity(Activity & a)
	{
		// refuse to add activity with the same title
		for (int i = 0; i < _numberOfActivities; i++)
			if (strcmp(a.gettitle(), _activities[i].gettitle()) == 0)
				return;

		Activity * temp = new Activity[_numberOfActivities + 1];
		for (int i = 0; i < _numberOfActivities; i++)
			temp[i].Copy(_activities[i]);
		temp[_numberOfActivities].Copy(a);
		_numberOfActivities++;
		if (_activities != nullptr)
			delete[] _activities;
		_activities = temp;
	}

	// Function that indicates activity at a certain position as completed
	void FinishActivity(int index)
	{
		if (index >= 0 && index < _numberOfActivities)
			_activities[index].FinishActivity();
	}

	//// The function that needs to assign a new employee to existing activity
	void AssignEmployee(Activity & a, Employee & u)
	{
		for (int i = 0; i < _numberOfActivities; i++)
		{
			if (strcmp(a.gettitle(), _activities[i].gettitle()) == 0)
			{
				_activities[i].AddEmployee(u);
				return;
			}
		}
	}

	// Function that needs to return all employeees engaged on a particular activity. The input parameter is activity title.
	Employee** ReturnAllEmployeesToTheActivity(char* activityTitle)
	{
		for (int i = 0; i < _numberOfActivities; i++)
			if (strcmp(activityTitle, _activities[i].gettitle()) == 0)
				return _activities[i].getEmployees();
		return nullptr;
	}

	//Funkciju koja uklanja određenog Employeea na activities. Ulazni parametri funkcije su title activities i šifra Employeea.
	//// Function that removes a particular employee from the activity. The input parameters of the function are activity title and the employee ID.
	void RemoveEmployeeFromActivity(char *activityTitle, int employeeID)
	{
		for (int i = 0; i < _numberOfActivities; i++)
			if (strcmp(activityTitle, _activities[i].gettitle()) == 0)
			{
				_activities[i].RemoveEmployee(employeeID);
				return;
			}
	}

	// Function that removes all Employers engaged on Activity with a subtitle sent as a parameter
	void RemoveAllEmployeesNaactivities(char* activityTitle)
	{
		for (int i = 0; i < _numberOfActivities; i++)
			if (strcmp(activityTitle, _activities[i].gettitle()) == 0)
			{
				_activities[i].RemoveAllEmployees();
				return;
			}
	}

	//Function that finds employee with the highest number of engagements on the activity
	Employee* GetTheBestEmployee()
	{
		Employee * theBest = nullptr;
		int theBestNumberOfActivities = 0;
		int tempNumberOfActivities = 0;
		Employee** arrayOfEmployeesTemp = nullptr;
		for (int i = 0; i < _numberOfActivities; i++)
		{
			arrayOfEmployeesTemp = _activities[i].getEmployees();
			for (int j = 0; j < 5; j++)
			{
				if (arrayOfEmployeesTemp[j] != nullptr)
				{
					tempNumberOfActivities = CountEmployeeActivities(arrayOfEmployeesTemp[j]);
					if (tempNumberOfActivities > theBestNumberOfActivities)
					{
						theBest = arrayOfEmployeesTemp[j];
						theBestNumberOfActivities = tempNumberOfActivities;
					}
				}
			}
		}
		return theBest;
	}

	// Function that finds 3 Employees engaged in the highest number of unfinished activities
	void GetThreeWorstEmployees(Employee** arrayWorst)
	{
		int arrayMaxUnfinished[3];
		int tempCounterUnfinished = 0;
		Employee** tempEmployees = nullptr;
		bool alreadyExists;
		for (int i = 0; i < 3; i++)
		{
			arrayMaxUnfinished[i] = 0;
			arrayWorst[i] = nullptr;
		}

		for (int i = 0; i < _numberOfActivities; i++)
		{
			if (_activities[i].IsFinished() == false)
			{
				tempEmployees = _activities[i].getEmployees();
				for (int j = 0; j < 5; j++)
				{
					if (tempEmployees[j] != nullptr)
					{
						//checks if the employee is already in the array of the worst ones
						alreadyExists = false;
						for (int k = 0; k < 3; k++)
							if (arrayWorst[k] != nullptr && arrayWorst[k]->getemployeeID() == tempEmployees[j]->getemployeeID())
								alreadyExists = true;
						if (alreadyExists == false)
						{
							tempCounterUnfinished = CountUnfinishedEmpoyeeActivities(tempEmployees[j]);

							//provjera da li zasluzuje biti u nizu najgorih
							//checks if employee deserves to be in the array of the worst ones
							for (int k = 0; k < 3; k++)
							{
								if (arrayMaxUnfinished[k] < tempCounterUnfinished)
								{
									//if employee deserves
									arrayWorst[k] = tempEmployees[j];
									arrayMaxUnfinished[k] = tempCounterUnfinished;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	//Function that prints all finished activities on the project
	void IspisiZavrseneactivities()
	{
		cout << "\nZavrsene activities\n";
		for (int i = 0; i < _numberOfActivities; i++)
			if (_activities[i].IsFinished())
				cout << "-" << _activities[i].gettitle() << endl;
	}

	//Added
	void Info()
	{
		cout << "\nProject:\n";
		cout << "Title: " << _title << endl;
		cout << "Section:" << _section << endl;
		cout << "Duration: " << _duration << " months" << endl;
		cout << "Number of activities: " << _numberOfActivities << endl;
		cout << "Activities: ";
		for (int i = 0; i < _numberOfActivities; i++)
			cout << _activities[i].gettitle() << ", ";
		cout << endl;
	}
	int CountEmployeeActivities(Employee * u)
	{
		int employeeID = u->getemployeeID();
		int counter = 0;
		Employee **employeedOnActivity = nullptr;
		for (int i = 0; i < _numberOfActivities; i++)
		{
			employeedOnActivity = _activities[i].getEmployees();
			for (int j = 0; j < 5; j++)
				if (employeedOnActivity[j] != nullptr && employeedOnActivity[j]->getemployeeID() == employeeID)
					counter++;
		}
		return counter;
	}
	int CountUnfinishedEmpoyeeActivities(Employee * u)
	{
		int employeeID = u->getemployeeID();
		int counter = 0;
		Employee **employeedOnActivity = nullptr;
		for (int i = 0; i < _numberOfActivities; i++)
		{
			if (_activities[i].IsFinished() == false)
			{
				employeedOnActivity = _activities[i].getEmployees();
				for (int j = 0; j < 5; j++)
					if (employeedOnActivity[j] != nullptr && employeedOnActivity[j]->getemployeeID() == employeeID)
						counter++;
			}
		}
		return counter;
	}
	int CountFinishedEmpoyeeActivities(Employee * u)
	{
		int employeeID = u->getemployeeID();
		int counter = 0;
		Employee **employeedOnActivity = nullptr;
		for (int i = 0; i < _numberOfActivities; i++)
		{
			if (_activities[i].IsFinished())
			{
				employeedOnActivity = _activities[i].getEmployees();
				for (int j = 0; j < 5; j++)
					if (employeedOnActivity[j] != nullptr && employeedOnActivity[j]->getemployeeID() == employeeID)
						counter++;
			}
		}
		return counter;
	}
	void FinishActivity(Activity & a)
	{
		a.FinishActivity();
		for (int i = 0; i < _numberOfActivities; i++)
			if (strcmp(a.gettitle(), _activities[i].gettitle()) == 0)
			{
				_activities[i].FinishActivity();
				return;
			}
	}

	int CountDurationOfFinishedActivities(Employee * u)
	{
		int employeeID = u->getemployeeID();
		int counter = 0;
		Employee **employeedOnActivity = nullptr;
		for (int i = 0; i < _numberOfActivities; i++)
		{
			if (_activities[i].IsFinished())
			{
				employeedOnActivity = _activities[i].getEmployees();
				for (int j = 0; j < 5; j++)
					if (employeedOnActivity[j] != nullptr && employeedOnActivity[j]->getemployeeID() == employeeID)
					{
						counter += _activities[i].getduration();
					}
			}
		}
		return counter;
	}

	int getduration() { return _duration; }
	int getcountertivityi() { return _numberOfActivities; }
	Activity * getactivities() { return _activities; }
	void getactivitiesnfo(Activity & a)
	{
		for (int i = 0; i < _numberOfActivities; i++)
			if (strcmp(a.gettitle(), _activities[i].gettitle()) == 0)
				_activities[i].Info();
	}
};



/*Create a global function that finds the most successful prospectus on projects transmitted as a parameter.
employee is considered the most successful if he is engaged in the largest number of completed activities. If more employees
have the same number of completed activities, it is more successful to consider the one whose activities lasted longer.
*/

Employee* GetTheMostSuccessfulEmployeeOnAllProjects(Project** projects)
{
	Employee * theBest = nullptr;
	int maxSumOfActivitiesDuration = -1;
	int MaxFinishedActivities = 0;

	Employee * tempEmployee = nullptr;
	int tempSumOfActivitiesDuration = -1;
	int tempFinishedActivities = 0;

	Activity* activities = nullptr;
	Employee** employees = nullptr;

	for (int i = 0; i < _numberOfProjects; i++)
	{
		activities = projects[i]->getactivities();
		for (int j = 0; j < projects[i]->getcountertivityi(); j++)
		{
			if (activities[j].IsFinished())
			{
				employees = activities[j].getEmployees();
				for (int k = 0; k < 5; k++)
				{
					if (employees[k] != nullptr)
					{
						tempEmployee = employees[k];
						tempFinishedActivities = 0;
						tempSumOfActivitiesDuration = 0;
						for (int a = 0; a < _numberOfProjects; a++)
						{
							tempFinishedActivities += projects[a]->CountFinishedEmpoyeeActivities(tempEmployee);
							tempSumOfActivitiesDuration += projects[a]->CountDurationOfFinishedActivities(tempEmployee);
							if (tempFinishedActivities > MaxFinishedActivities)
							{
								theBest = tempEmployee;
								MaxFinishedActivities = tempFinishedActivities;
								maxSumOfActivitiesDuration = tempSumOfActivitiesDuration;
							}
							else if (tempFinishedActivities == MaxFinishedActivities && maxSumOfActivitiesDuration < tempSumOfActivitiesDuration)
							{
								theBest = tempEmployee;
								MaxFinishedActivities = tempFinishedActivities;
								maxSumOfActivitiesDuration = tempSumOfActivitiesDuration;
							}
						}
					}
				}
			}
		}
	}
	return theBest;
}

int main()
{
	//Input data for employees
	Employee Coma(1111, "Coma", "Comic", "Evulate Company");
	Employee Kip(2222, "Kip", "Thorne", "Farnod Company");
	Employee Kathy(3333, "Kathy", "Niakan", "Chaims Company");
	Employee Gina(4444, "Gina", "Moreno", "Datlu Company");
	Employee Christiana(5555, "Christiana", "Figueres", "Endster Company");
	Employee Alan(6666, "Alan", "Stern", "Gnany Company");
	Employee Eve(7777, "Eve", "Murad", "Tranello Company");
	Employee Dan(8888, "Dan", "Carder", "Rapidora Company");
	Employee Sunita(9999, "Sunita", "Narain", "Nolunt Company");
	//Christiana.Info();


	//Project1 and its activities
	Project webPage("Web Page", "Web", 2);

	Activity templates("Templates and Components", 4);
	Activity html("HTML intergration", 2);
	Activity multiLang("Multi-language", 1);
	Activity testing("Functional Testing", 3);
	Activity bugFix("Bug-Fixing", 2);
	webPage.AddActivity(templates);
	webPage.AddActivity(html);
	webPage.AddActivity(multiLang);
	webPage.AddActivity(testing);
	webPage.AddActivity(bugFix);

	//Distribution of jobs
	webPage.AssignEmployee(templates, Alan);
	webPage.AssignEmployee(templates, Eve);
	webPage.AssignEmployee(html, Sunita);
	webPage.AssignEmployee(html, Kip);
	webPage.AssignEmployee(html, Dan);
	webPage.AssignEmployee(multiLang, Kathy);
	webPage.AssignEmployee(multiLang, Gina);
	webPage.AssignEmployee(multiLang, Alan);
	webPage.AssignEmployee(testing, Christiana);
	webPage.AssignEmployee(testing, Eve);
	webPage.AssignEmployee(bugFix, Gina);
	//webPage.Info();


	//Project2 and its activities
	Project mobileApp("Mobile app", "Phones", 3);

	Activity storage("Data Storage", 10);
	Activity management("User management", 13);
	Activity design("UI design", 12);
	Activity test("test", 11);
	mobileApp.AddActivity(storage);
	mobileApp.AddActivity(management);
	mobileApp.AddActivity(design);
	mobileApp.AddActivity(test);
	//mobileApp.Info();

	//Distribution of jobs
	mobileApp.AssignEmployee(storage, Christiana);
	mobileApp.AssignEmployee(management, Alan);
	mobileApp.AssignEmployee(design, Sunita);
	mobileApp.AssignEmployee(design, Alan);
	mobileApp.AssignEmployee(test, Kathy);
	//mobileApp.getactivitiesnfo(design);




	//Finishing some acitivies in both projects 
	webPage.FinishActivity(templates);
	webPage.FinishActivity(html);
	webPage.FinishActivity(testing);

	mobileApp.FinishActivity(storage);
	mobileApp.FinishActivity(management);
	mobileApp.FinishActivity(design);


	//mobileApp.getactivitiesnfo(test);
	//mobileApp.RemoveEmployeeFromActivity("test", 3333);
	//mobileApp.getactivitiesnfo(test);

	//webPage.IspisiZavrseneactivities();
	//mobileApp.IspisiZavrseneactivities();



	// ******************************************** ISPIS ********************************************

	//first project
	webPage.Info();
	Employee * theBestEmployee = webPage.GetTheBestEmployee();
	cout << endl << "The best employee of webPage project is " << theBestEmployee->getname() << ". Employee is currently engaged on " << webPage.CountEmployeeActivities(theBestEmployee) << " activities of this project." << endl;
	cout << crt;

	//second project
	mobileApp.Info();
	theBestEmployee = mobileApp.GetTheBestEmployee();
	cout << endl << "The best employee of mobileApp project is " << theBestEmployee->getname() << ". Employee is currently engaged on " << mobileApp.CountEmployeeActivities(theBestEmployee) << " activities of this project." << endl;
	cout << crt;

	//total
	Project *allProjects[2];
	allProjects[0] = &webPage;
	allProjects[1] = &mobileApp;

	theBestEmployee = GetTheMostSuccessfulEmployeeOnAllProjects(allProjects);
	cout << "\n\t\t***** THE MOST SUCCESSFUL employee on ALL projects is " << theBestEmployee->getname() << ".*****";
	cout << "\n\nEmployee has in total " << allProjects[0]->CountFinishedEmpoyeeActivities(theBestEmployee) + allProjects[1]->CountFinishedEmpoyeeActivities(theBestEmployee) << " finished activities on all projects together.";


	system("pause>0");
	return 0;
}