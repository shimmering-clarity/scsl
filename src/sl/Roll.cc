#include <iostream>
#include <string>
#include <vector>

#include <scccl/math/math.h>
using namespace std;
using namespace scmath;


static void
rollDie(char *s)
{
	int	m = 0, n = 0;
	int	i = 0;
	bool	readSides = false;

	while (s[i] != '\0') {
		if (s[i] != 'd' && !isdigit(s[i])) {
			cerr << "Invalid die specification!" << endl;
			return;
		}

		if (readSides) {
			if (s[i] == 'd') {
				cerr << "Invalid die specification!" << endl;
				return;
			}
			n *= 10;
			n += (s[i] - 0x30);
		} else {
			if (s[i] == 'd') {
				readSides = true;
			} else {
				m *= 10;
				m += (s[i] - 0x30);
			}
		}

		i++;
	}

	if (m == 0) {
		m = 1;
	}

	cout << s << ": " << DieTotal(m, n) << endl;
}


static void
rollPlayer()
{
	vector<string>	statNames = {"STR", "CON", "DEX", "INT", "PER"};
	vector<int>	statRolls;

	for (size_t i = 0; i < statNames.size(); i++) {
		statRolls.push_back(BestDie(3, 4, 6));
	}

	for (size_t i = 0; i < statNames.size(); i++) {
		cout << statNames[i] << ": " << statRolls[i] << endl;
	}
}


int
main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		if (string(argv[i]) == "player") {
			rollPlayer();
		} else {
			rollDie(argv[i]);
		}
	}
}
