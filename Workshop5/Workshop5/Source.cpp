#include<iostream>
using namespace std;
char *crt = "\n------------------------------------------\n";

class TimeT
{
	int _hours;
	int _minutes;
	int _seconds;
public:
	TimeT(int hours, int minutes, int seconds = 0) {
		_hours = hours;
		_minutes = minutes;
		_seconds = seconds;
	}
	//operator << for printing object of TimeT type
	friend ostream& operator<<(ostream&, TimeT&);
	friend TimeT operator+(TimeT&, int);
	TimeT& operator+=(int newMinutes)
	{
		*this = *this + newMinutes;
		return *this;
	}
	friend bool operator<(TimeT&, TimeT&);
	friend bool operator>(TimeT&, TimeT&);

};
ostream& operator<<(ostream& COUT, TimeT& TimeT)
{
	if (TimeT._seconds == 0)
		return COUT << TimeT._hours << ":" << TimeT._minutes;
	return COUT << TimeT._hours << ":" << TimeT._minutes << ":" << TimeT._seconds;
}
TimeT operator+(TimeT& v, int newMinutes)
{
	int minutesTotal = v._hours * 60 + v._minutes + newMinutes;
	return TimeT(minutesTotal / 60, minutesTotal % 60, v._seconds);
}
bool operator<(TimeT& v1, TimeT& v2)
{
	int t1 = v1._hours * 3600 + v1._minutes * 60 + v1._seconds;
	int t2 = v2._hours * 3600 + v2._minutes * 60 + v2._seconds;
	return (t1 < t2);
}
bool operator>(TimeT& v1, TimeT& v2)
{
	int t1 = v1._hours * 3600 + v1._minutes * 60 + v1._seconds;
	int t2 = v2._hours * 3600 + v2._minutes * 60 + v2._seconds;
	return (t1 > t2);
}

class Flight
{
	static const  int _indexSize = 10;
	char _index[_indexSize];
	char* _destination;
	int _gateNumber;
	TimeT _timeOfTakingOff; //hours and minutes
	int _duration; //expressed in minutes
	int _delay; //expressed in minutes

public:
	//constructor and destructor functions
	Flight(char* index, char* destination, int gateNumber, TimeT& timeOfTakingOff, int duration, int delay = 0) :
		_timeOfTakingOff(timeOfTakingOff)
	{
		strncpy_s(_index, index, _indexSize);
		int size = strlen(destination) + 1;
		_destination = new char[size];
		strcpy_s(_destination, size, destination);
		_gateNumber = gateNumber;
		_duration = duration;
		_delay = delay;
	}
	~Flight()
	{
		delete[] _destination;
		_destination = nullptr;
	}

	// Function with one parameter that sets information about eventual delay on the default parameter value.
	void Setdelay(int newDelay)
	{
		_delay = newDelay;
	}

	/* Logical negation operator "!" through which it is possible to find out if the corresponding flight is late or not (return logic
		value "true" in case of delay, otherwise return "false".) */
	friend bool operator!(const Flight&);

	// Function that return duration of the flight as a result
	int GetFlightDuration()
	{
		return _duration;
	}

	/* operator + in a way that allows you to add "Flight" type objects to the totsl number at
		resulting in a new "Flight" facility, in which the time of taking off is moved
		in advance for the amount determined by another saber (counted in minutes).
		Also fold the operator "+ =" which ensures that the form expression "X + = Y always has the same meaning as the expression
		"X = X + Y" */
	friend Flight operator+(Flight&, int);
	Flight& operator+=(int newMinutes)
	{
		_timeOfTakingOff = _timeOfTakingOff + newMinutes;
		return *this;
	}

	/* Function to find out the expected time of taking off when calculating the amount of delay
		in relation to the anticipated time of taking off (use operator +). */
	TimeT GetExpectedTimeOfTakingOff()
	{
		return _timeOfTakingOff + _delay;
	}

	// Function that returns expected time of landing
	TimeT GetExpectedTimeOfLanding()
	{
		return _timeOfTakingOff + _delay + _duration;
	}

	/* Operator "++" in a way that reminds time of departure for one hour in advance.
		It is also necessary to support the prefix and postfix version of this operator. */
	Flight& operator++()
	{
		//pre increment
		_timeOfTakingOff + 60;
		return *this;
	}
	Flight& operator++(int)
	{
		//postincrement
		Flight temp(_index, _destination, _gateNumber, _timeOfTakingOff, _duration, _delay);
		_timeOfTakingOff += 60;
		return temp;
	}

	/*Link Relational Operators "<" and ">" to examine which flight occurs earlier or later.
	The operator "<" returns the logical value "true" if the takeoff of flight to the left is performed
	before taking flight to the right, otherwise it returns the "false" logical value.
	Analogously it applies to the operator ">". When comparing, the expected time of delay should be considered,
	not just scheduled time of takeoff. */
	friend bool operator<(Flight&, Flight&);
	friend bool operator>(Flight&, Flight&);

	/* operator "<<" to support the printout of Flight type objects on screen. In case of
	it is about leaving without delays, the printout should look like this:

	JFK 156 Atalanta 12 : 50 19 : 30 5

	The data represent the order : flight, destination name, takeoff time, expected
	Landing time and number of exit gate. In case of delayed flight, the printout should look like this :

	ZGK 932 Zagreb 15 : 50 (Planned 15:30, Late 20 min) */

	friend ostream& operator<<(ostream&, Flight&);

	//Added
	char* GetFlightIndex() { return _index; }
};

ostream& operator<<(ostream& COUT, Flight& Flight)
{
	if (!Flight) // if it's late
		return COUT << Flight._index << "\t" << Flight._destination << "\t" << Flight.GetExpectedTimeOfTakingOff() << "\t" << "(Planned " << Flight._timeOfTakingOff << ", late " << Flight._delay << " min)";
	return COUT << Flight._index << "\t" << Flight._destination << "\t" << Flight.GetExpectedTimeOfTakingOff() << "\t" << Flight.GetExpectedTimeOfLanding() << "\t" << Flight._gateNumber;
}
bool operator!(const Flight& Flight)
{
	return Flight._delay != 0;
}
Flight operator+(Flight& l, int addedMinutes)
{
	return Flight(l._index, l._destination, l._gateNumber, l._timeOfTakingOff + addedMinutes, l._duration, l._delay);
}
bool operator<(Flight& l1, Flight& l2)
{
	return l1.GetExpectedTimeOfTakingOff() < l2.GetExpectedTimeOfTakingOff();
}
bool operator>(Flight& l1, Flight& l2)
{
	return l1.GetExpectedTimeOfTakingOff() > l2.GetExpectedTimeOfTakingOff();
}

class FlightSchedule
{
	int _numberOfRegisteredFlights;
	const int _maxNumberOfFlights;
	Flight** _Flights;

public:
	FlightSchedule(int maxNumberOfFlights) : _maxNumberOfFlights(maxNumberOfFlights),
		_Flights(new Flight*[_maxNumberOfFlights]), _numberOfRegisteredFlights(0)
	{}
	~FlightSchedule()
	{
		delete[] _Flights;
		_Flights = nullptr;
		_numberOfRegisteredFlights = 0;
	}

	/* Operator "+=" in a way to register a new flight schedule. The schedule of flights at each moment should be sorted according to take off time.
	Take care of the maximum number of flights. For the purpose of comparing flight departure times, fold down the appropriate operators in the TimeT class. */
	FlightSchedule& operator+=(Flight& newFlight)
	{
		if (_numberOfRegisteredFlights == _maxNumberOfFlights)
			return *this;
		int newIndex = 0;
		for (int i = 0; i < _numberOfRegisteredFlights; i++)
		{	// comparing the take-off times to know where to put the new flight
			if (_Flights[i]->GetExpectedTimeOfTakingOff() > newFlight.GetExpectedTimeOfTakingOff())
				newIndex = i + 1;
		}
		// move certain flights to put a new flight on the correct index and finally embark on a new flight in a series of flights
		for (int i = _numberOfRegisteredFlights; i > newIndex; i--)
			_Flights[i] = _Flights[i - 1];
		_Flights[newIndex] = &newFlight;
		_numberOfRegisteredFlights++;
		return *this;
	}

	/* operator "- =" in a way to remove a registered flight with the tag defined by the right operand.
     Preserve the order of the previously registered flight. */
	FlightSchedule& operator-=(Flight& previousFlight)
	{
		for (int i = 0; i < _numberOfRegisteredFlights; i++)
		{
			if (strcmp(_Flights[i]->GetFlightIndex(), previousFlight.GetFlightIndex()) == 0)
			{
				for (int j = i; j < _numberOfRegisteredFlights - 1; j++)
					_Flights[j] = _Flights[j + 1];
				_numberOfRegisteredFlights--;
				return *this;
			}
		}
	}

	// operator "[]" in a way to return the Flight type object based on the forwarded position in the schedule (positions range from 1)
	Flight& operator[](int index)
	{
		if (index > 0 || index <= _numberOfRegisteredFlights)
		{
			index--;
			return *_Flights[index];
		}
	}

    //operator "()" in a way that flight with a defined tag moves time of takeing off for the value of another parameter expressed in minutes
	FlightSchedule& operator()(char* index, int addedMinutes)
	{
		for (int i = 0; i < _numberOfRegisteredFlights; i++)
			if (strcmp(_Flights[i]->GetFlightIndex(), index) == 0)
			{
				_Flights[i]->Setdelay(addedMinutes);
				return *this;
			}
		return *this;
	}

	//Function that gives the average duration of all saved flights
	float GetAverageDurationOfAllFlights()
	{
		int sum = 0;
		for (int i = 0; i < _numberOfRegisteredFlights; i++)
			sum += _Flights[i]->GetFlightDuration();
		return sum / (float)_numberOfRegisteredFlights;
	}

	/* operator "<<" to print a list of all flights sorted by expected departure times.
		For the purposes of comparing the flight landing time, fold the appropriate operators in the TimeT class. */
	friend ostream& operator<<(ostream&, FlightSchedule&);
};

ostream& operator<<(ostream& COUT, FlightSchedule& FlightSchedule)
{
	for (int i = FlightSchedule._numberOfRegisteredFlights; i > 0; i--)
		COUT << FlightSchedule[i] << endl; //can be printed like this because operator[] has been defined
	return COUT;
}


int main()
{
	TimeT morning(6, 30), forenoon(10, 15), noon(12, 0), afternoon(16, 55), evening(20, 40), midnight(0, 0);
	//cout << forenoon;

	Flight Flight1("OU3417", "Zagreb  ", 2, afternoon, 30);
	Flight Flight2("OS7603", "Vienna  ", 7, forenoon, 120);
	Flight Flight3("TK1022", "Istanbul", 2, morning, 222);
	Flight Flight4("LH1731", "Munich  ", 4, midnight, 230);
	Flight Flight5("FZ7628", "Dubai   ", 1, noon, 112);
	Flight Flight6("JU1131", "Belgrade", 5, evening, 44);
	Flight Flight7("SK8945", "Ljubljana", 4, afternoon, 55);
	//cout << Flight5;

	FlightSchedule FlightSchedule(10);
	FlightSchedule += Flight1;
	FlightSchedule += Flight2;
	FlightSchedule += Flight3;
	FlightSchedule += Flight4;
	FlightSchedule += Flight5;
	FlightSchedule += Flight6;
	FlightSchedule += Flight7;

	FlightSchedule("JU1131", 45); // == Flight6.Setdelay(45);  //can be done this way because operator() has been defined


								  /***********************PRINT******************************/

	cout << crt << "\t::FLIGHT SCHEDULE::" << crt << FlightSchedule << endl;
	//cout << "\n\nAveragre duration of fligths is " << (int)FlightSchedule.GetAverageDurationOfAllFlights() << " minutes" << endl;

	system("pause>0");
	return 0;
}
