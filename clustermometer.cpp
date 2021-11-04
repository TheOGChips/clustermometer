//#include <iostream>
#include <fstream>
#include <string>
//#include <cstdio>
//#include <array>
#include <mpi.h>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

const int MASTER = 0;
const string CPU_TEMP_FILE = "/sys/class/thermal/thermal_zone0/temp";	//same in all Raspberry Pis

//void run_display (int&);
void run_display (float[], const int, atomic<bool>&);
void increment_num (int&);
void get_master_cpu_temp (float&, const atomic<bool>&);
float read_cpu_temp (/*array<ifstream, NUM_CORES>&, array<string, NUM_CORES>*/);
float to_celsius (float);
float celsius_to_fahrenheit (float);

int main()
{
	MPI_Init(nullptr, nullptr);
	
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (rank == MASTER) {
		//int temp = 0;
		//thread thr (increment_num, ref(temp));
		atomic<bool> stop (false);
		int world_size;
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		
		float cpu_temps[world_size];
		
		
		thread thr (get_master_cpu_temp, ref(cpu_temps[MASTER]), ref(stop));
		//run_display(temp);
		run_display(cpu_temps, world_size, stop);
		thr.join();
	}
	
	else {
		//get_cpu_temp();	//currently, other nodes do nothing
	}
	
	//array<string, NUM_CORES> filenames;
	//array<ifstream, NUM_CORES> infiles;
	
	MPI_Finalize();
	return 0;
}

//void run_display (int &count)
void run_display (float cpu_temps[], const int WORLD_SIZE, atomic<bool> & stop)
{
	initscr();				//initialize ncurses environment
	curs_set(0);			//turn off the cursor
	nodelay(stdscr, true);	//make getch() nonblocking
	noecho();				//turn off displaying keyboard input
	
	//string DEGREES_SYMBOL = "";
	
	printw("Press 'q' to finish...\n\n");
	while (getch() != 'q') {
		for (int i = 0; i < WORLD_SIZE; i++) {
			//printw("Node %d: %.1f %sC\r", i, cpu_temps[i], DEGREES_SYMBOL.c_str());
			printw("Node %d: %.1f F\r", i, cpu_temps[i]);
		}
		//printw("count: %d\r", count);
		refresh();
	}
	
	endwin();		//terminate ncurses environment
	stop = true;	//signal slave thread to terminate
}
/*
void increment_num (int &count)
{
	//for (int i = 0; i < 6; i++) {
	while (!stop) {
		this_thread::sleep_for(chrono::seconds(1));
		count++;
	}
}
*/
void get_master_cpu_temp (float & cpu_temp, const atomic<bool> & STOP)
{
	//if master node
	while (!STOP) {
		cpu_temp = read_cpu_temp();
		cpu_temp = to_celsius(cpu_temp);
		cpu_temp = celsius_to_fahrenheit(cpu_temp);
		this_thread::sleep_for(chrono::seconds(1));	//prevents race condition for output
	}
	
	//else (if slave node)
		//get temperature (mC)
		//convert to C
		//convert to F
		//TODO: Send local CPU temp back to master if a slave node
}

void get_slave_cpu_temp ()
{
	
}

float read_cpu_temp (/*array<ifstream, NUM_CORES> & infiles, array<string, NUM_CORES> filenames*/)
{
	ifstream infile;
	infile.open(CPU_TEMP_FILE.c_str());
	
	float temp;
	infile >> temp;
	
	infile.close();
	return temp;
}

float to_celsius (float mC)
{
	return mC / 1000;	// milliCelsius to Celsius
}

float celsius_to_fahrenheit (float celsius)
{
	return celsius * 1.8 + 32;	// Fahrenheit <- Celsius * (9 / 5) + 32
}
