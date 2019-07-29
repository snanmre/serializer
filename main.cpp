#include <iostream>
#include <iomanip>

#include "serializer/Bundle.h"
#include "serializer/Serializable.h"

using namespace std;



class A: public Serializable {

public:
	A(int a = 0, bool b = false, double c = 0) : a(a), b(b), c(c)
	{
	}

	void printValues() {
		cout << "a = " << a << endl
			 << "b = " << b << endl
			 << "c = " << c << endl
			 << "d =";
		for (auto s : d)
			cout << " " << s;
		cout << endl;
	}
	int a;
	bool b;
	double c;
	vector<string> d;

private:

	virtual void writeToBundle(Bundle & bundle) const {
		bundle << a << b << c << d;
	}
	virtual void readFromBundle(Bundle & bundle) {
		bundle >> a >> b >> c;
		bundle.getArray(d);
	}
};



int main(int argc, char **argv)
{
	///////////////////  test with variables

	cout << "variable serializations..." << endl << endl;

	int a = 13, aa;
	bool b = true, bb;
	double c = 4232252.32329, cc;
	string d = "abcdefghijklmnopqrstuvwxyz 1234567890 !'^+%&/()=?_ >£#$½¾{[]}\\|", dd;

	Bundle bundle;

	cout << "serialized values:" << endl
		 << "a = " << a << endl
		 << "b = " << b << endl
		 << "c = " << std::fixed << c << endl
		 << "d = " << d << endl
		 << endl
		 << "deserialized values:" << endl;

	// serialize
	bundle << a << b << c << d;

//	int len = bundle.byteCount();
//	bundle.exportData(u8buf, size);
//	bundle.importData(u8buf, len)

	while (bundle.count())
	{
		switch (bundle.getNextType())
		{
		case Bundle::TYPE_INT:
			aa = bundle.getInt();
			cout << "int       => " << aa << endl;
			break;

		case Bundle::TYPE_BOOL:
			bb = bundle.getBool();
			cout << "bool      => " << bb << endl;
			break;

		case Bundle::TYPE_DOUBLE:
			bundle >> cc;
			cout << "double    => " << cc << endl;
			break;

		case Bundle::TYPE_STRING:
			bundle >> dd;
			cout << "string    => " << dd << endl;
			break;

		default:
			cerr << "ERROR: undef type" << endl;
			exit(1);
		}
	}



	/////////////// test with objs

	bundle.clear();

	cout << endl << endl << "Object serialization..." << endl << endl;

	A obj(523, false, 42523.23242);
	obj.d.push_back("qwerty");
	obj.d.push_back("asdfgh");
	obj.d.push_back("zxcvbn");

	cout << "original obj:" << endl;
	obj.printValues();

	bundle << obj;

	A obj2;
	bundle >> obj2;
	cout << endl << "serialized obj:" << endl;
	obj2.printValues();

	return 0;
}
