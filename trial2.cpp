#include <iostream>
#include "joblist.h"
#include "list.h"
#include "Queue.h"
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
//Changes i made to case 1 is at both for loop put i=1 and not i=0
int main()
{
	string filename;
	int Job_num, arrv_time, Pros_time, Job_size;
	fstream inputFile;
	joblist obj[100]; // 
	list memory_partition1; // memory partition lists are created
	list memory_partition2;
	//Queue waiting_q; // a waiting queue is declared here
	ifstream infile;
	int array_size, option;
	int total_jobs;
	int simulation_end_time = 0; // to get the final time for the simulation
	int num_job_x_masuk_cpu = 0;
	float total_frag = 0;
	int max_queue_length = 0;
	int min_queue_length = 0;
	cout << "Enter the following menu to test your current programme" << endl;
	cout << "1) Dislay Joblist" << endl;
	cout << "2) Display MemoryList" << endl;
	cout << "3) Fixed Partition first fit" << endl;
	cout << "4) Fixed Partition best-fit" << endl;
	cout << "---> Enter your option:";
	cin >> option;
	switch (option)
	{
	case 1:
	{
		//	cout << "Enter the filename : ";	cin >> filename;
		//	filename += ".txt";
		filename = "joblist.txt";
		inputFile.open(filename, ios::in);
		if (inputFile)
		{
			inputFile >> total_jobs;
			//	cout << "\nTotal number of jobs from this file is : " << total_jobs << endl;
			for (int i = 1; i <= total_jobs; i++)
			{
				inputFile >> Job_num;
				inputFile >> arrv_time;
				inputFile >> Pros_time;
				inputFile >> Job_size;
				obj[i].set_job_num(Job_num);
				obj[i].set_arrival(arrv_time);
				obj[i].set_pros_time(Pros_time);
				obj[i].set_jobs(Job_size);
			}
		}
		else
		{
			cout << "wrong file input " << endl;
		}
		for (int i = 1; i <= total_jobs; i++)
		{
			/*		cout << obj[i].get_job() << '\t';
					cout << obj[i].get_arrival() << '\t';
					cout << obj[i].get_pros() << '\t';
					cout << obj[i].get_jobsize() << '\t';
					cout << endl;	*/
		}
		//break;
	}
	case 2:
	{
		//	cout << "\nENTER TEXT FILENAME (memory) : ";
		//	cin >> filename;	filename = filename + ".txt";
		filename = "memorylist.txt";
		infile.open(filename.c_str(), ios::in);
		while (!infile) // to check the availabilty of the entered file
		{
			cout << "\n>> COULD'NT OPEN THE ENTERED TEXT FILE <<";
			cout << "\n\nENTER TEXT FILENAME AGAIN : ";
			cin >> filename;
			infile.open(filename.c_str(), ios::in);
		}
		if (infile) // read the first line from the textfile
			infile >> array_size;
		int memory_size_array[50]; // to know how many memory partitions there will be inside
		if (infile)
		{
			system("CLS");
			for (int i = 0; i < array_size && !infile.eof(); i++)
			{
				infile >> memory_size_array[i]; // will read the memory size from the textfile
				infile.ignore();  // to ignore the "enter" thing we pressed at textfile
			}
			infile.close();
		}
		// memory size is stored into the array..
		// now how to create the linked list according to the memory..............
		for (int i = 0; i < array_size; i++)
		{
			memory_partition1.appendlist(memory_size_array[i], i); // the value from the memory list is assigned into each partition
		}
		//	cout << "\n\nGonna display all of it again (not sorted)::" << endl;
	//	memory_partition1.displaylist();
		// ascending order is from kecik ke besar..for best fit...
	//	cout << "\n\nGonna display all of it again (sorted, ascending order)::" << endl;
		for (int i = 0; i < array_size; i++)
		{
			memory_partition2.sortedlist(memory_size_array[i],i); // the value from the memory list is assigned into each partition
		}
		//	memory_partition2.displaylist();
			//	cout << "\n\n\nThts the end of it, pergi tido man !!!" << endl;
				//break;
	}
	case 3:
	{
		int status = 0, new_arrival = 0, mem_no = 0, counter = 1; // works as a flag to indicate the status of memory
		Queue test_waiting_q, second_waiting_q;	// waiting queue is created
		bool flag1, bo = true; // to check whether the job has entered the memory or not
		int exit_time = 0;
		int wait_time = 0;
		int internal_frag = 0;
		int new_queue_length = 0;
		int mem_blocks;
		
		cout << "Enter the number of memory blocks you want to test. Only this memory blocks are available to test" << endl;	//Enter valid memory blocks only
		cout << "3\n5\n7\n10" << endl;
		cin >> mem_blocks;
		while (mem_blocks != 3 && mem_blocks != 5 && mem_blocks != 7 && mem_blocks != 10)
		{
			cout << "incorrect choosing of memory blocks. Please choose again." << endl;
			cout << "Enter the number of memory blocks you want to test. Only this memory blocks are available to test" << endl;
			cout << "3\n5\n7\n10" << endl;
			cin >> mem_blocks;
		}

		for (int j = 1; j <= 40; j++) // j is the job pointer..
		{
			if (new_queue_length >= max_queue_length)
				max_queue_length = new_queue_length;
			if (new_queue_length <= min_queue_length)
				min_queue_length = new_queue_length;
			flag1 = true;	// to check if the job got a partition or not
			for (int k = 1; k <= mem_blocks; k++) // k is the fixed memory pointer
			{
				if (obj[j].get_jobsize() <= memory_partition1.show_mem_size(k))	// to know job less than memory or not
				{
					while (bo) // bo returns true if process time < arrival time , it will run while this condition stands , end of the list = returns false
					{
						memory_partition1.departure(obj[j].get_arrival(), new_arrival, mem_no, counter, bo);	// check wther b4 current job arrival is thr any job finish processed or not
						if (counter == 1)	//if there is memory departured will check the queue
						{
							while (!test_waiting_q.isEmpty()) // run the queue until it all sdh check
							{
								int value;
								test_waiting_q.dequeue(value);
								if (obj[value].get_jobsize() <= memory_partition1.show_mem_size(mem_no)) // check the queued job size less than memory which is freed from previous job only
								{
									exit_time = new_arrival + obj[value].get_pros();	//total process time. the new arrival time get from the departure of old job
									obj[value].set_job_completion_time(exit_time);
									wait_time = new_arrival - obj[value].get_arrival(); // determine the waiting time for that job
									obj[value].set_job_waiting_time(wait_time);
									memory_partition1.set_status(mem_no, exit_time, obj[value].get_job());	//set the memory into bz
									//simulation_end_time = exit_time;
									internal_frag = memory_partition1.show_mem_size(mem_no) - obj[value].get_jobsize();
									total_frag = total_frag + internal_frag;
									cout << "\nt = " << new_arrival << " from queue, job number " << value << " sized " << obj[value].get_jobsize() << " has entered memory " << mem_no;
									cout << " sized " << memory_partition1.show_mem_size(mem_no) << endl;
									system("PAUSE");
									break;
								}
								else
								{
									second_waiting_q.enqueue(value);	//if not fit into memory masuk balik into the second queue. To avoid repetition here.
								}
							}
							while (!second_waiting_q.isEmpty())	//eventually, the second queue data will sent to the test queue.
							{
								int value;
								second_waiting_q.dequeue(value);
								test_waiting_q.enqueue(value);
							}
							counter = 0;	//reset the counter
						}
					}
					bo = true;
					status = memory_partition1.listcheck(k, obj[j].get_arrival());	//check whether the memmory is bz or free
					if (status == 1)	// if bz then skip the loop to find next available memory
					{
						continue;
					}
					else if (status == 0)	// the memory is free and good to proceed
					{
						cout << "\n\nAt t = " << obj[j].get_arrival() << " job number " << obj[j].get_job() << " size : " << obj[j].get_jobsize();
						cout << " has entered the memory number " << memory_partition1.show_mem_num(k);
						cout << " size : " << memory_partition1.show_mem_size(k) << endl;
						exit_time = obj[j].get_arrival() + obj[j].get_pros(); // get the exit time
						memory_partition1.set_status(k, exit_time, obj[j].get_job());
						obj[j].set_job_completion_time(exit_time);
						obj[j].set_job_waiting_time(0);
						//simulation_end_time = exit_time;
						internal_frag = memory_partition1.show_mem_size(k) - obj[j].get_jobsize();
						total_frag = total_frag + internal_frag;
						cout << "this job will exit at t = " << obj[j].get_job_completion_time() << endl;
						flag1 = false; // false = sdh dpt tempat
						system("PAUSE");						
						break; // exit the k for loop	
					}
				}
			}
			if (flag1) // the job tht didnt get any memory partition will enter the quueu via this function
			{
				cout << "\n\nAt t = " << obj[j].get_arrival() << " job number " << obj[j].get_job() << " with the size of  ";
				cout << obj[j].get_jobsize() << " has entered the queue " << endl;
				test_waiting_q.enqueue(obj[j].get_job());
				new_queue_length = test_waiting_q.get_quantity();
				system("PAUSE");
			}
			if (j == 40)	//after job 40 thr is no arrival so after job 40 it will display all remaining processes
			{
				while (bo) // bo returns true if process time < arrival time , it will run while this condition stands , end of the list = returns false
				{
					memory_partition1.departure(1000, new_arrival, mem_no, counter, bo);	// check whether b4 current job arrival is thr any job finish processed or not
					if (counter == 1)	//if there is memory departured will check the queue
					{
						while (!test_waiting_q.isEmpty()) // run the queue until it all sdh check
						{
							int value;
							test_waiting_q.dequeue(value);
							if (obj[value].get_jobsize() <= memory_partition1.show_mem_size(mem_no)) // check the queued job size less than memory which is freed from previous job only
							{
								exit_time = new_arrival + obj[value].get_pros();	//total process time. the new arrival time get from the departure of old job
								obj[value].set_job_completion_time(exit_time);
								wait_time = new_arrival - obj[value].get_arrival(); // determine the waiting time for that job
								obj[value].set_job_waiting_time(wait_time);
								memory_partition1.set_status(mem_no, exit_time, obj[value].get_job());	//set the memory into bz
								simulation_end_time = exit_time;
								internal_frag = memory_partition1.show_mem_size(mem_no) - obj[value].get_jobsize();
								total_frag = total_frag + internal_frag;
								cout << "\nt = " << new_arrival << " from queue, job number " << value << " sized " << obj[value].get_jobsize() << " has entered memory " << mem_no;
								cout << " sized " << memory_partition1.show_mem_size(mem_no) << endl;
								system("PAUSE");
								break;
							}
							else
							{
								second_waiting_q.enqueue(value);	//if not fit into memory masuk balik into the second queue. To avoid repetition here.
							}
						}
						while (!second_waiting_q.isEmpty())	//eventually, the second queue data will sent to the test queue.
						{
							int value;
							second_waiting_q.dequeue(value);
							test_waiting_q.enqueue(value);
						}
						counter = 0;	//reset the counter
						if (simulation_end_time < new_arrival)
							simulation_end_time = new_arrival;
					}
				}
			}
			bo = true;
		}


		cout << "\n\n>>> Jobs that are waiting in the queue <<< " << endl;
		while (!test_waiting_q.isEmpty())	//display the job number in queue;
		{
			int value;
			test_waiting_q.dequeue(value);
			num_job_x_masuk_cpu++;
			cout << "job number : " << value;
			cout << " job size : " << obj[value].get_jobsize() << endl;
		}
		float total_wait_time = 0;
		float total_process_time = 0;
		for (int f = 1; f <= 40; f++)
		{
			if (obj[f].get_job_waiting_time() >= 0)
			{
				total_wait_time = total_wait_time + obj[f].get_job_waiting_time();
				total_process_time = total_process_time + obj[f].get_pros();
			}
		}
		cout << "\n--> total waiting time : " << total_wait_time;
		cout << "\n---> average waiting time in the queue : " << (total_wait_time / (40 - num_job_x_masuk_cpu));
		cout << "\n----> the simulation time : " << simulation_end_time;
		cout << "\n-----> total processing time : " << total_process_time;
		cout << "\n------> throughput value : " << (total_process_time / simulation_end_time);
		cout << "\n-------> average queue length (max) : " << max_queue_length;
		cout << "\n-------> average queue length (min) : " << min_queue_length;
		cout << "\n--------> total internal fragmentation : " << total_frag;
		cout << "\n---------> average internal fragmentation : " << (total_frag / (40 - num_job_x_masuk_cpu));
		cout << "\n";
		//system("pause");
	}
	system("pause");
	case 4:
	{
		int status = 0, new_arrival = 0, mem_no = 0, counter = 1; // works as a flag to indicate the status of memory
		Queue test_waiting_q, second_waiting_q;	// waiting queue is created
		bool flag1, bo = true; // to check whether the job has entered the memory or not
		int exit_time = 0;
		int wait_time = 0;
		int simulation_time = 0;
		int internal_frag = 0, sum_frag = 0;
		int new_queue_length = 0;
		int mem_blocks;
		int max_que_length = 0;
		int min_que_length = 0;
		int num_job_no_masuk_cpu = 0;

		cout << "Enter the number of memory blocks you want to test. Only this memory blocks are available to test" << endl;	//Enter valid memory blocks only
		cout << "3\n5\n7\n10" << endl;
		cin >> mem_blocks;
		while (mem_blocks != 3 && mem_blocks != 5 && mem_blocks != 7 && mem_blocks != 10)
		{
			cout << "incorrect choosing of memory blocks. Please choose again." << endl;
			cout << "Enter the number of memory blocks you want to test. Only this memory blocks are available to test" << endl;
			cout << "3\n5\n7\n10" << endl;
			cin >> mem_blocks;
		}

		for (int j = 1; j <= 40; j++) // j is the job pointer..
		{
			if (new_queue_length >= max_que_length)
				max_que_length = new_queue_length;
			if (new_queue_length <= min_que_length)
				min_que_length = new_queue_length;
			flag1 = true;	// to check if the job got a partition or not
			for (int k = 1; k <= mem_blocks; k++) // k is the fixed memory pointer
			{
				if (obj[j].get_jobsize() <= memory_partition2.show_mem_size(k))	// to know job less than memory or not
				{
					while (bo) // bo returns true if process time < arrival time , it will run while this condition stands , end of the list = returns false
					{
						memory_partition2.departure(obj[j].get_arrival(), new_arrival, mem_no, counter, bo);	// check wther b4 current job arrival is thr any job finish processed or not
						if (counter == 1)	//if there is memory departured will check the queue
						{
							while (!test_waiting_q.isEmpty()) // run the queue until it all sdh check
							{
								int value;
								test_waiting_q.dequeue(value);
								if (obj[value].get_jobsize() <= memory_partition2.show_mem_size(mem_no)) // check the queued job size less than memory which is freed from previous job only
								{
									exit_time = new_arrival + obj[value].get_pros();	//total process time. the new arrival time get from the departure of old job
									obj[value].set_job_completion_time(exit_time);
									wait_time = new_arrival - obj[value].get_arrival(); // determine the waiting time for that job
									obj[value].set_job_waiting_time(wait_time);
									memory_partition2.set_status(mem_no, exit_time, obj[value].get_job());	//set the memory into bz
									//simulation_end_time = exit_time;
									internal_frag = memory_partition2.show_mem_size(mem_no) - obj[value].get_jobsize();
									sum_frag = sum_frag + internal_frag;
									cout << "\nt = " << new_arrival << " from queue, job number " << value << " sized " << obj[value].get_jobsize() << " has entered memory " << mem_no;
									cout << " sized " << memory_partition2.show_mem_size(mem_no) << endl;
									system("PAUSE");
									break;
								}
								else
								{
									second_waiting_q.enqueue(value);	//if not fit into memory masuk balik into the second queue. To avoid repetition here.
								}
							}
							while (!second_waiting_q.isEmpty())	//eventually, the second queue data will sent to the test queue.
							{
								int value;
								second_waiting_q.dequeue(value);
								test_waiting_q.enqueue(value);
							}
							counter = 0;	//reset the counter
						}
					}
					bo = true;
					status = memory_partition2.listcheck(k, obj[j].get_arrival());	//check whether the memmory is bz or free
					if (status == 1)	// if bz then skip the loop to find next available memory
					{
						continue;
					}
					else if (status == 0)	// the memory is free and good to proceed
					{
						cout << "\n\nAt t = " << obj[j].get_arrival() << " job number " << obj[j].get_job() << " size : " << obj[j].get_jobsize();
						cout << " has entered the memory number " << memory_partition2.show_mem_num(k);
						cout << " size : " << memory_partition2.show_mem_size(k) << endl;
						exit_time = obj[j].get_arrival() + obj[j].get_pros(); // get the exit time
						memory_partition2.set_status(k, exit_time, obj[j].get_job());
						obj[j].set_job_completion_time(exit_time);
						obj[j].set_job_waiting_time(0);
						//simulation_end_time = exit_time;
						internal_frag = memory_partition2.show_mem_size(k) - obj[j].get_jobsize();
						sum_frag = sum_frag + internal_frag;
						cout << "this job will exit at t = " << obj[j].get_job_completion_time() << endl;
						flag1 = false; // false = sdh dpt tempat
						system("PAUSE");
						break; // exit the k for loop	
					}
				}
			}
			if (flag1) // the job tht didnt get any memory partition will enter the quueu via this function
			{
				cout << "\n\nAt t = " << obj[j].get_arrival() << " job number " << obj[j].get_job() << " with the size of  ";
				cout << obj[j].get_jobsize() << " has entered the queue " << endl;
				test_waiting_q.enqueue(obj[j].get_job());
				new_queue_length = test_waiting_q.get_quantity();
				system("PAUSE");
			}
			if (j == 40) //after job 40 thr is no arrival so after job 40 it will display all remaining processes
			{
				while (bo) // bo returns true if process time < arrival time , it will run while this condition stands , end of the list = returns false
				{
					memory_partition2.departure(1000, new_arrival, mem_no, counter, bo);	// check whether b4 current job arrival is thr any job finish processed or not
					if (counter == 1)	//if there is memory departured will check the queue
					{
						while (!test_waiting_q.isEmpty()) // run the queue until it all sdh check
						{
							int value;
							test_waiting_q.dequeue(value);
							if (obj[value].get_jobsize() <= memory_partition2.show_mem_size(mem_no)) // check the queued job size less than memory which is freed from previous job only
							{
								exit_time = new_arrival + obj[value].get_pros();	//total process time. the new arrival time get from the departure of old job
								obj[value].set_job_completion_time(exit_time);
								wait_time = new_arrival - obj[value].get_arrival(); // determine the waiting time for that job
								obj[value].set_job_waiting_time(wait_time);
								memory_partition2.set_status(mem_no, exit_time, obj[value].get_job());	//set the memory into bz
								simulation_time = exit_time;
								internal_frag = memory_partition2.show_mem_size(mem_no) - obj[value].get_jobsize();
								sum_frag = sum_frag + internal_frag;
								cout << "\nt = " << new_arrival << " from queue, job number " << value << " sized " << obj[value].get_jobsize() << " has entered memory " << mem_no;
								cout << " sized " << memory_partition2.show_mem_size(mem_no) << endl;
								system("PAUSE");
								break;
							}
							else
							{
								second_waiting_q.enqueue(value);	//if not fit into memory masuk balik into the second queue. To avoid repetition here.
							}
						}
						while (!second_waiting_q.isEmpty())	//eventually, the second queue data will sent to the test queue.
						{
							int value;
							second_waiting_q.dequeue(value);
							test_waiting_q.enqueue(value);
						}
						counter = 0;	//reset the counter
						if (simulation_time < new_arrival)
							simulation_time = new_arrival;
					}
				}
			}
			bo = true;
		}


		cout << "\n\n>>> Jobs that are waiting in the queue <<< " << endl;
		while (!test_waiting_q.isEmpty())	//display the job number in queue;
		{
			int value;
			test_waiting_q.dequeue(value);
			num_job_no_masuk_cpu++;
			cout << "job number : " << value;
			cout << " job size : " << obj[value].get_jobsize() << endl;
		}
		float total_wait_time = 0;
		float total_process_time = 0;
		for (int f = 1; f <= 40; f++)
		{
			if (obj[f].get_job_waiting_time() >= 0)
			{
				total_wait_time = total_wait_time + obj[f].get_job_waiting_time();
				total_process_time = total_process_time + obj[f].get_pros();
			}
		}
		cout << "\n--> total waiting time : " << total_wait_time;
		cout << "\n---> average waiting time in the queue : " << (total_wait_time / (40 - num_job_no_masuk_cpu));
		cout << "\n----> the simulation time : " << simulation_time;
		cout << "\n-----> total processing time : " << total_process_time;
		cout << "\n------> throughput value : " << (total_process_time / simulation_time);
		cout << "\n-------> average queue length (max) : " << max_que_length;
		cout << "\n-------> average queue length (min) : " << min_que_length;
		cout << "\n--------> total internal fragmentation : " << sum_frag;
		cout << "\n---------> average internal fragmentation : " << (sum_frag / (40 - num_job_no_masuk_cpu));
		cout << "\n";
		system("pause");
	}
	case 5:
	{
		//dynamic partition
		int mem_size,category, bestfit = 0, worstfit = 0, fragment = 0, total_exfrag = 0, total_exfrag1 = 0, total_exfrag2 = 0, total_exfrag3 = 0, simulation_time = 0;
		list memory_parti;
		int value = 0, status = 0, status1 = 0, check = 0, check1 = 0, status2 = 0,newarr = 0;
		Queue waiting, waiting2;
		bool bo = true;

		cout << "DYNAMIC PARTITION" << endl;

		cout << "Enter 1 for best-fit or 2 for worst-fit or 3 for first-fit" << endl;
		cin >> category;
		while (category != 1 && category != 2 && category != 3)
		{
			cout << "Incorrect choosing. Please choose again." << endl;
			cout << "Enter 1 for best-fit or 2 for worst-fit  or 3 for first-fit" << endl;
			cin >> category;
		}
		if (category == 1)
			bestfit = 1;
		else if (category == 2)
			worstfit = 1;
		
		cout << "Enter the memory partition you want to test.\n20000\n30000\n40000\n50000" << endl;
		cin >> mem_size;
		while (mem_size != 20000 && mem_size != 30000 && mem_size != 40000 && mem_size != 50000)	//choose memory size
		{
			cout << "Invalid memory partition. Please enter again." << endl;
			cout << "Enter the memory partition you want to test.\n20000\n30000\n40000\n50000" << endl;
			cin >> mem_size;
		}

		memory_parti.appendlist1(mem_size);	//append memory size in link list

		for (int i = 1; i <= 40; i++)
		{
			while (bo)	//check whole list
			{
				memory_parti.dyndeparture(obj[i].get_arrival(),newarr, check, bo,bestfit,worstfit); //check job departure
				if (check == 1) //if departure, then memory free
				{
					while (!waiting.isEmpty())
					{
						int num;
						waiting.dequeue(num);
						memory_parti.dynjob1(obj[num].get_jobsize(), newarr, obj[num].get_pros(), obj[num].get_job(), status1, value, bestfit, worstfit,fragment);	//queue jobs insertion into memory
						total_exfrag1 = total_exfrag1 + fragment;
						fragment = 0;
						if (status1 == 1)	//if queue job not fit into memory
						{
							waiting2.enqueue(num);
						}
						status1 = 0;
					}
					while (!waiting2.isEmpty())	//queue back job to original queue
					{
						int number;
						waiting2.dequeue(number);
						waiting.enqueue(number);
					}
				}
				check = 0;
			}
			bo = true;
			memory_parti.dynjob(obj[i].get_jobsize(), obj[i].get_arrival(), obj[i].get_pros(),obj[i].get_job(),status,value, bestfit, worstfit,fragment);	//insert job into memory
			total_exfrag2 = total_exfrag2 + fragment;
			fragment = 0;
			if (status == 1)	//job cannot fit into memory
			{
				waiting.enqueue(value);	//job number
			}
			status = 0;
			if (i == 40) //if no more job arrive,it continue departures all the remaining jobs
			{
				while (bo)	//check the whole memory list
				{
					memory_parti.dyndeparture(1000,newarr, check1, bo, bestfit, worstfit);
					if (check1 != 0)
					{
						while (!waiting.isEmpty())
						{
							int num;
							waiting.dequeue(num);
							memory_parti.dynjob1(obj[num].get_jobsize(), newarr, obj[num].get_pros(), obj[num].get_job(), status2, value, bestfit, worstfit,fragment);
							total_exfrag3 = total_exfrag3 + fragment;
							fragment = 0;
							if (status2 == 1)
							{
								waiting2.enqueue(num);
							}
							status2 = 0;
						}
						while (!waiting2.isEmpty())
						{
							int number;
							waiting2.dequeue(number);
							waiting.enqueue(number);
						}
						if (simulation_time < newarr)	//calculate total simulation time
							simulation_time = newarr;
					}
					check1 = 0;
				}
				bo = true;
			}
		}
		cout << "Job in waiting queue:" << endl;
		while (!waiting.isEmpty())
		{
			int value;
			waiting.dequeue(value);
			cout << "Job number " << obj[value].get_job() << " sized " << obj[value].get_jobsize() << endl;
		}
		bestfit = 0; worstfit = 0;
		total_exfrag = total_exfrag1 + total_exfrag2 + total_exfrag3;
		cout << "Total external fragmentation: " << total_exfrag << endl;
		cout << "Simulation time: " << simulation_time << endl;
	}
	}
}