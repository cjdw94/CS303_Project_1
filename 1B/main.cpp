#include "Library.h"

using namespace std;

void main() {
	Library library;
	library.add_book("Software Engineering");
	library.add_book("Chemistry");

	library.add_employee("Adam");
	library.add_employee("Sam");
	library.add_employee("Ann");

	library.circulate_book("Chemistry", Date(2015, 3, 1, DateFormat::US));
	library.circulate_book("Software Engineering", Date(2015, 4, 1, DateFormat::US));

	library.pass_on("Chemistry", Date(2015, 3, 5, DateFormat::US)); 
	library.pass_on("Chemistry", Date(2015, 3, 7, DateFormat::US));
	library.pass_on("Chemistry", Date(2015, 3, 15, DateFormat::US)); 
	library.pass_on("Software Engineering", Date(2015, 4, 5, DateFormat::US));
	library.pass_on("Software Engineering", Date(2015, 4, 10, DateFormat::US));
	library.pass_on("Software Engineering", Date(2015, 4, 15, DateFormat::US));

	system("pause");
}