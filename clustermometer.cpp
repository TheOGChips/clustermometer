#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <array>
//#include <mpi.h>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

const int MASTER = 0;
const string CPU_TEMP = "/sys/class/thermal/thermal_zone0/temp";
atomic<bool> stop (false);

void run_display (int &num);
void increment_num (int &num);
//void set_filenames (array<string, NUM_CORES>&);
//void open_files (array<ifstream, NUM_CORES>&, array<string, NUM_CORES>);
//void close_files (array<ifstream, NUM_CORES>&);
//float get_temp (array<ifstream, NUM_CORES>&, array<string, NUM_CORES>);
//void display_temps (array<ifstream, NUM_CORES>&, array<string, NUM_CORES>);

int main()
{
	int temp = 0;
	thread thr (increment_num, ref(temp));
	run_display(temp);
	thr.join();
	//array<string, NUM_CORES> filenames;
	//array<ifstream, NUM_CORES> infiles;
	
	//MPI_Init();
	
	//int rank;
	//MPI_Comm_rank(&rank, MPI_COMM_WORLD);
	
	//if (rank == MASTER) {
		//thread display (record_temps, )
	//}
	//set_filenames(filenames);
	//display_temps(infiles, filenames);
	
	//MPI_Finalize();
	return 0;
}

void run_display (int &count)
{
	initscr();				//initialize ncurses environment
	curs_set(0);			//turn off the cursor
	nodelay(stdscr, true);	//make getch() nonblocking
	noecho();				//turn off displaying keyboard input
	
	printw("Press 'q' to finish...\n\n");
	while (getch() != 'q') {
		printw("count: %d\r", count);
		refresh();
	}
	
	endwin();		//terminate ncurses environment
	stop = true;	//signal slave thread to terminate
}

void increment_num (int &count)
{
	//for (int i = 0; i < 6; i++) {
	while (!stop) {
		this_thread::sleep_for(chrono::seconds(1));
		count++;
	}
}

/*void set_filenames (array<string, NUM_CORES> & filenames)
{
	for (int i = 0; i < NUM_CORES; i++) {
		filenames[i] = "/sys/class/thermal/thermal_zone" + to_string(i) + "/temp";
	}
}*/
/*
void open_files (array<ifstream, NUM_CORES> & infiles, array<string, NUM_CORES> filenames)
{
	for (int i = 0; i < NUM_CORES; i++) {
		infiles[i].open(filenames[i]);
	}
}

void close_files (array<ifstream, NUM_CORES> & infiles)
{
	for (int i = 0; i < NUM_CORES; i++) {
		infiles[i].close();
	}
}

float get_temp (array<ifstream, NUM_CORES> & infiles, array<string, NUM_CORES> filenames)
{
	open_files(infiles, filenames);
	
	//array<float, NUM_CORES> cpu_temps;
	float avg_cpu_temp = 0.0;
	for (int i = 0; i < NUM_CORES; i++) {
		float temp;
		infiles[i] >> temp;
		avg_cpu_temp += temp;
	}
	avg_cpu_temp /= NUM_CORES;
	
	close_files(infiles);
	return avg_cpu_temp;
}

void display_temps (array<ifstream, NUM_CORES> & infiles, array<string, NUM_CORES> filenames)
{
	cpu_temp = record_temps(infiles, filenames);
	
	//for (int i = 0; i < NUM_CORES; i++) {
	//	cout << "Core " << i << " temp: " << cpu_temps[i] << endl;
	//}
}
*/
