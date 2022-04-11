/*  main.c  - main */

#include <xinu.h>

int sector_number = 20;

void exe1(void);
void exe2(void);
void exe3(void);
void exe4(void);

void reader(void);
void writer(char *);


int main(int argc, char ** argv)
{
	//kprintf("Main process: %d, name %s\n", currpid, proctab[currpid].prname);
	kprintf("\n\nCS503 Lab3 \n\r");
	kprintf("Initializing the disk ...\n");
	disk();

	kprintf("\n--------------Running exe1-------------\n\r");
	exe1();

	kprintf("\n--------------Running exe2-------------\n\r");
        exe2();

	kprintf("\n--------------Running exe3-------------\n\r");
        exe3();

	kprintf("\n--------------Running exe4-------------\n\r");
        exe4();
	
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
}

void reader() 
{
	char sector_out[SECTOR_SIZE];
	if (read_sector(sector_number, sector_out))
		kprintf("Read %s from sector %d\n", sector_out, sector_number);
	else 
		kprintf("Unable to read sector %d\n", sector_number);
}

void writer(char * input) 
{
	char sector_in[SECTOR_SIZE];
	strlen(input) >= SECTOR_SIZE ? strncpy(sector_in, input, SECTOR_SIZE) : strcpy(sector_in, input);
	kprintf("Writer writes %s for 50 times\n", input);
	for (int i = 0; i < 50; i++) 
		write_sector(sector_number, sector_in);
}

void exe1() 
{
	resume( create(writer, 2000, 30, "writerA", 1, "A"));
	kprintf("flushing writer A\n");
	flush();
	sleep(1);
	resume( create(writer, 2000, 30, "writerB", 1, "B"));
	flush();
	kprintf("flushing writer B\n");
	sleep(1);
	resume(create(reader, 2000, 30, "reader", 0));
} 

void exe2()
{
	resume( create(writer, 2000, 30, "writerA", 1, "A"));
	resume( create(writer, 2000, 30, "writerB", 1, "B"));
	flush();
	kprintf("flushing\n");
	sleep(1);
	resume(create(reader, 2000, 30, "reader", 0));
}

void exe3()
{
	resume( create(writer, 2000, 30, "writerA", 1, "A"));
	flush();
	resume( create(writer, 2000, 30, "writerB", 1, "B"));
	sleep(1);
        resume(create(reader, 2000, 30, "reader", 0));
}


void exe4()
{
	kprintf("Before Power failure:\n");
	resume( create(writer, 2000, 30, "writerA", 1, "A: Crashing Soon"));
        resume( create(writer, 2000, 30, "writerB", 1, "B: Oh no someone save me. Please!!"));
        flush();
	inject_failure();
        sleep(1);
	kprintf("After power failure:\n");
	resume(create(reader, 2000, 30, "reader", 0));
        resume( create(writer, 2000, 30, "writerC", 1, "C: LOL, I'm the new content after the power failure"));
	flush();
	resume(create(reader, 2000, 30, "reader", 0));
}
