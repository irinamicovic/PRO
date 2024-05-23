#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

char students[166][60];
int points[166][4], index[166];
int no_names = 0, no_surnames = 0;

int generatePoints()
{
	return rand() % 101;
}

void sortTable(char students[][60], int index[], int points[][4])
{
	int i, j, n, temp;
	char tempc[30];

	for (i = 1; i <= 166; i++)
	{
		for (n = i + 1; n <= 166; n++)
		{
			if (points[i][3] < points[n][3])
			{
				temp = points[n][3];
				points[n][3] = points[i][3];
				points[i][3] = temp;

				temp = points[n][2];
				points[n][2] = points[i][2];
				points[i][2] = temp;

				temp = points[n][1];
				points[n][1] = points[i][1];
				points[i][1] = temp;

				temp = points[n][0];
				points[n][0] = points[i][0];
				points[i][0] = temp;

				temp = index[n];
				index[n] = index[i];
				index[i] = temp;

				for (j = 0; j < 30; j++)
					tempc[j] = students[n][j];
				for (j = 0; j < 30; j++)
					students[n][j] = students[i][j];
				for (j = 0; j < 30; j++)
					students[i][j] = tempc[j];
			}
		}
	}

}

void generateNames(char names[][30], char surnames[][30])
{
	int b, temp, i, j;

	for (i = 1; i <= 166; i++)
	{
		temp = rand();
		for (j = 0; names[temp % no_names][j] != '\0'; j++)
		{
			students[i][j] = names[temp % no_names][j];
			b = j;
		}
		students[i][j + 1] = ' ';

		for (j = 0; surnames[temp % no_surnames][j] != '\0'; j++)
		{
			students[i][b + 2] = surnames[temp % no_surnames][j];
			b++;
		}
		students[i][b + 2] = '\0';
	}

}

int findClosest(float target, int arr[][2], int n)
{
	float closest = fabs(target - arr[1][1]);
	int ind, oc;

	printf("Closest at sequential no. :");
	for (int i = 2, b = 0; i <= n; i++)
	{
		if (fabs((target - (float)arr[i][1])) <= closest)
		{
			closest = fabs(arr[i][1] - target);
			ind = arr[i][0];
			oc = arr[i][1];
		}
	}

	for (int i = 1; i <= n; i++)
	{
		if (arr[i][1] == oc)
			cout << arr[i][0] << "  ";
	}
}

int main()
{
	int i, j;
	char names[30][30]{ "Aleksandra","Marta","Veljko","Luka","Matej","Natalija","Nemanja","Sasa","Nina","Leona","Andjela","Viktor","Relja","Sanja","Nevena","Pavle","Milan","Stefan","Dusan","Kosta","Miona","Jovan","Strahinja","Irina","Andrea","Konstantin","Djurdja","Mia","Maja","David" };
	char surnames[20][30]{ "Milic","Micovic","Djurovic","Vuckovic","Janjic","Gunjic","Vucinic","Drobnjak","Paunovic","Ivic","Djusic","Matovic","Veskovic","Stevanovic","Senicic","Jablanov","Golic","Jevtovic","Dragojevic","Petrovic" };
	int p, no_pass = 0;
	float AVG, OC = 0;
	int passed[166][2];

	srand(time(0));


	for (i = 1; i <= 166; i++)
	{
		index[i] = i;
	}

	for (i = 1; i <= 166; i++)
	{
		for (j = 0; j < 3; j++)
		{
			points[i][j] = generatePoints();
		}
		p = points[i][0] + points[i][1] + points[i][2];

		if (p >= 150 && p <= 179)
			points[i][3] = 6;
		else if (p >= 180 && p <= 209)
			points[i][3] = 7;
		else if (p >= 210 && p <= 239)
			points[i][3] = 8;
		else if (p >= 240 && p <= 269)
			points[i][3] = 9;
		else if (p >= 270 && p <= 300)
			points[i][3] = 10;
		else
			points[i][3] = 5;
	}


	for (i = 0, no_names = 0; names[i][0] != '\0'; i++)
	{
		no_names++;
	}
	for (i = 0, no_surnames = 0; surnames[i][0] != '\0'; i++)
	{
		no_surnames++;
	}
	if (no_names != 0 && no_surnames != 0)
		generateNames(names, surnames);
	else
	{
		cout << "No name or surname in the arrays!";
		exit(0);
	}

	cout << "Index   Name and Surname     KV   RV   VI   OC\n";
	sortTable(students, index, points);

	for (i = 1; i <= 166; i++)
	{
		printf("%3d	", index[i]);
		for (j = 0; j < 20; j++)
		{
			cout << students[i][j];
		}

		if (points[i][3] == 5)
		{
			passed[i][0] = -1;
			passed[i][1] = -1;
			if (points[i][1] < 50 && points[i][2] < 50)
				cout << points[i][0] << "   " << points[i][1] << "   " << points[i][2] << "    " << "RV,VI";

			else if (points[i][1] < 50)
				cout << points[i][0] << "   " << points[i][1] << "   " << points[i][2] << "    " << "RV";

			else if (points[i][2] < 50)
				cout << points[i][0] << "   " << points[i][1] << "   " << points[i][2] << "    " << "VI";

		}
		else
		{
			OC += points[i][3];
			no_pass++;

			passed[i][0] = index[i];
			passed[i][1] = points[i][3];

			for (j = 0; j < 4; j++)
				cout << points[i][j] << "   ";
		}
		cout << endl;

	}

	if (no_pass == 0)
		cout << "No one has passed the exam!";

	else
	{
		AVG = OC / no_pass;
		cout << "Average grade od students that passed the exam is " << AVG << endl;
		findClosest(AVG, passed, no_pass);
	}

	return 0;

}
