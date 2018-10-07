#include<iostream>
#include<fstream>
#include<random>

using namespace std;

int main()
{
	float GetRandomNumber(int x, int y);

	float lamda = 2.0 / 60;// arrival rate
	float lamda2 = 1.0 / 25;// service rate

	float arriveTime = 0;
	float timeInterval = 0.0;

	float serviceTime1 = 0.0;
	float serviceTime2 = 0.0;
	
	float departureTime1 = 0.0;
	float departureTime2 = 0.0;

	serviceTime1 = GetRandomNumber(0, 25);// uniform distribution
	serviceTime2 = -log(GetRandomNumber(0, 1)) / lamda2;

	departureTime1 = arriveTime + serviceTime1;
	departureTime2 = arriveTime + serviceTime2;

	ofstream file1;
	ofstream file2;
	file1.open("output_result1.csv");
	file2.open("output_result2.csv");

	for (int i = 0; i < 10000; i++)
	{
		timeInterval = -log(GetRandomNumber(0, 1)) / lamda;
		arriveTime += timeInterval;

		serviceTime1 = GetRandomNumber(0, 25);// uniform distribution
		serviceTime2 = -log(GetRandomNumber(0, 1)) / lamda2;

		/*departureTime1 = arriveTime + serviceTime1;
		departureTime2 = arriveTime + serviceTime2;*/

	
		
		if (arriveTime >= departureTime1)
		{
			departureTime1 = arriveTime + serviceTime1;
		}
		else
		{
			departureTime1 = departureTime1 + serviceTime1;
		}

		if (arriveTime >= departureTime2)
		{
			departureTime2 = arriveTime + serviceTime2;
		}
		else
		{
			departureTime2 = departureTime2 + serviceTime2;
		}

		if (arriveTime > 4 * 3600)// simulation time horizon
		{
			break;
		}
		file1 << i + 1 << "," << arriveTime << "," << departureTime1 << endl;// output the result in csv file
		file2 << i + 1 << "," << arriveTime << "," << departureTime2 << endl;
	}
}

float GetRandomNumber(int x, int y)
{
	random_device rd;   // non-deterministic generator
	mt19937 gen(rd());  // to seed mersenne twister.
	uniform_real<float> dist(x, y); // distribute results between x and y inclusive.
	return dist(gen);
}