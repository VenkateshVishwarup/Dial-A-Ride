void floydwarshall(int);
int min(int, int);
void check_taxi(int, int, int);
int sort(int, int, int, int);

struct taxi_detail
{
	int no;
	int capacity;
	int location;
	int time;
	int revenue;
};

struct taxi_service
{
	int from_location;
	int to_location;
	int value;
};

struct output
{
	int taxi_no;
	int request_no;
	int time; 
	int from_location;
	int to_location;
	int revenue;
};

struct request
{
	int flag;
	int no;
	int from_location;
	int to_location;
	int from_time;
	int to_time;
};

struct close_taxi
{
	int id;
	int distance;
};
