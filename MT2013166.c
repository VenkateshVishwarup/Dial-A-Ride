#include<stdio.h>
#include<stdlib.h>
#include"header.h"
#define max 250
#define ntaxi 400
#define nrequest 10000

int city_detail[max][max];
struct taxi_detail taxi[ntaxi];
struct close_taxi cl_taxi[ntaxi];
struct output output[nrequest];
struct request request_det[nrequest];
struct taxi_service t_service[ntaxi][ntaxi];

void main(int argc, char *argv[] )
{

	if ( argc != 2 ) 								// To check that there should be 2 inputs only 
		printf( "Please enter valid argument ex. ./a.out <i/pfilename>.txt ");
	else
	{
	 	FILE *fp,*fp1;
	 	int no_of_cities=0;
	 	int line_track = 1;
	 	char buf[500];
	 	int capacity=0;
		int c;
		int no_of_taxi;
		int no_of_request;		
	 	int tmp, i=0, j=0, k=0;
		int revenue=0;
	  	fp = fopen(argv[1], "r");

	   	// Get the detail of the sizes of the array
		if(fp == NULL)  	
		{
	    		puts("Cannot open file for reading");
	    		exit(EXIT_FAILURE);
	  	}
	 
	 	while(line_track < 2) 
	    	{
			// This line will give us the sizes of different things 
	    		fgets(buf, sizeof(buf), fp);
	    		sscanf(buf, "%d %d %d %d", &no_of_cities, &no_of_taxi, &capacity, &no_of_request);
	    		line_track++;
	    	}
	 	
		int taxi_location[no_of_taxi];
	
		// Get the no of city and its distance to other cities
		for(i=0; i<no_of_cities; i++)
		{ 
		// This is used to get the city distance input in the array
			for(j=0; j<no_of_cities; j++)
			{
				fscanf(fp,"%d",&c);
				city_detail[i][j] = c;
			}
		}

		floydwarshall(no_of_cities);

		// Get the detail of all the taxi at night
		for(j=0; j<no_of_taxi; j++)
		{
			// This is to get the last night location of the array
			fscanf(fp,"%d",&c);
			taxi[j].location = c-1;
			taxi[j].no = j;
			taxi[j].capacity = 0;
			taxi[j].time = 0;
		}

		// Get the request information
		for(j=0; j<no_of_request; j++)
		{
			// This is to get the input of the request 
			fscanf(fp,"%d",&request_det[j].from_location);
			request_det[j].from_location = request_det[j].from_location -1;
			fscanf(fp,"%d",&request_det[j].to_location);
			request_det[j].to_location = request_det[j].to_location -1;
			fscanf(fp,"%d",&request_det[j].from_time);
			fscanf(fp,"%d",&request_det[j].to_time);
			request_det[j].no = j+1; 
			request_det[j].flag = 0;
		}
	 	fclose(fp);

		// sort the array on the basis of the form time
		for(i=0; i<no_of_request; i++)
		{
		// This loop is to sort the request according the request time and give us the sorted list
			for(j=i+1; j<no_of_request; j++)
			{
					if(request_det[i].from_time>request_det[j].from_time)
					{
		    				tmp = request_det[i].from_time;
		    				request_det[i].from_time = request_det[j].from_time;
		    				request_det[j].from_time = tmp;
						tmp = request_det[i].from_location;
						request_det[i].from_location = request_det[j].from_location;
						request_det[j].from_location = tmp;
						tmp = request_det[i].to_location;
						request_det[i].to_location = request_det[j].to_location;
						request_det[j].to_location = tmp;
						tmp = request_det[i].to_time;
						request_det[i].to_time = request_det[j].to_time;
						request_det[j].to_time = tmp;
						tmp = request_det[i].no;
						request_det[i].no = request_det[j].no;
						request_det[j].no = tmp;
					}				
			}
		}

		int distance,count=0;
		int l,m,n,loop,c3=0,c2=0,c1=0;
		int a[no_of_taxi],b[no_of_taxi],temp;
		int loop_for_output = 0;
		for(i=0; i<no_of_request; i++)
		{
			if(request_det[i].flag == 0)
			{
				// to create the array of taxi closest to the location
				check_taxi(request_det[i].from_location, no_of_taxi, request_det[i].from_time);				
				
				// Run a loop for the taxi to check which taxi will get the request
				for (j=0; j<no_of_taxi; j++)
				{
					c2++;
					if((taxi[cl_taxi[j].id].time + cl_taxi[j].distance*2 ) <= request_det[i].to_time && taxi[cl_taxi[j].id].capacity <= capacity)				
				// If the taxi reaches before the end time of request and if it has space this request will be served by this taxi
					{
						request_det[i].flag = 1;

						// Drop at pick
					
						int drop = taxi[cl_taxi[j].id].capacity;
					
						for(drop=0; drop < taxi[cl_taxi[j].id].capacity; drop++)
						{
							if(t_service[cl_taxi[j].id][drop].to_location == request_det[i].from_location)
							{
								taxi[cl_taxi[j].id].capacity = taxi[cl_taxi[j].id].capacity - 1;
								break;
							}
						}


						// Output is saved in the output structure
						output[i].taxi_no = taxi[cl_taxi[j].id].no ;
						output[i].request_no = request_det[i].no;
						output[i].from_location = request_det[i].from_location;
						output[i].to_location = request_det[i].to_location;
						output[i].time = (taxi[cl_taxi[j].id].time + cl_taxi[j].distance*2);
				
				// If the taxi reaches the location before the time user wants it to come it waits for user so the output value should be updated
						if(output[i].time < request_det[i].from_time)
							output[i].time = request_det[i].from_time;

		 
						// The arrray in which the request is saved for furthur processing
					
						t_service[cl_taxi[j].id][drop].from_location = request_det[i].from_location;
						t_service[cl_taxi[j].id][drop].to_location = request_det[i].to_location;
						//t_service[cl_taxi[j].id][taxi[cl_taxi[j].id].capacity].value = i;
					
						taxi[cl_taxi[j].id].capacity = taxi[cl_taxi[j].id].capacity + 1;

						taxi[cl_taxi[j].id].location = request_det[i].from_location;
						taxi[cl_taxi[j].id].time = taxi[cl_taxi[j].id].time + cl_taxi[j].distance*2;

						// If the taxi reaches the location before the time user wants it to come it waits for user
						if((taxi[cl_taxi[j].id].time)< request_det[i].from_time)
							taxi[cl_taxi[j].id].time = request_det[i].from_time;
				
						// Revenue is calculated from the distance to travel
						output[i].revenue = city_detail[output[i].from_location][output[i].to_location];

						// Revenue for the taxi is also calculated
						taxi[output[i].taxi_no].revenue = taxi[output[i].taxi_no].revenue + city_detail[output[i].from_location][output[i].to_location];
						// Total revenue is also updated
						revenue = revenue + output[i].revenue;
					

						// before releasing two condition's are checked first if the capacity of the taxi is zero and second is that it is the last request
						if(taxi[cl_taxi[j].id].capacity == capacity)
						{	
						// Loop to process all the request 
							int location, c[ntaxi];
							location = taxi[cl_taxi[j].id].location;
							l=capacity;
							for(k=capacity; k>0; k--)
							{
								for(m=1; m<=l; m++)
								{
									a[m] = city_detail[location][t_service[cl_taxi[j].id][m].to_location];
									b[m] = m;
								}

								for(m=1; m<=l; m++)
								{
									for(n=m+1; n<=l; n++)
									{
											if(a[m]>a[n])
											{
								    				temp = a[m];
								    				a[m] = a[n];
								    				a[n] = temp;
												temp = b[m];
								    				b[m] = b[n];
								    				b[n] = temp;
											}
									}
								}
						
								// sort the location to get the closest location where it can go
							
								// Update time as time taken to reach the place
								taxi[cl_taxi[j].id].time = taxi[cl_taxi[j].id].time + a[1] * 2;

								// Update time as time taken to reach the place
								taxi[cl_taxi[j].id].location = t_service[cl_taxi[j].id][b[1]].to_location;
								location = t_service[cl_taxi[j].id][b[1]].to_location;
								// Decrease value of loop
								l--;

								// Update the array
								t_service[cl_taxi[j].id][b[1]].to_location = t_service[cl_taxi[j].id][l].to_location;
								//l--;
								// Decrease the capacity of Taxi
								taxi[cl_taxi[j].id].capacity = taxi[cl_taxi[j].id].capacity -1;

								// Pick at drop 
								// Check if there is a request which can be picked at that time from that location, which will also satisfy the time constraint 
								for(loop=i; loop < no_of_request; loop++)
								{
									if(request_det[loop].flag == 0)
									{
										if(taxi[cl_taxi[j].id].time >= request_det[loop].from_time && taxi[cl_taxi[j].id].time <= request_det[loop].to_time)
										{
											output[loop].taxi_no = taxi[cl_taxi[j].id].no ;
											output[loop].request_no = request_det[loop].no;
											output[loop].from_location = request_det[loop].from_location;
											output[loop].to_location = request_det[loop].to_location;
											output[loop].time = taxi[cl_taxi[j].id].time;
											request_det[loop].flag = 1;

											
											t_service[cl_taxi[j].id][l].to_location = request_det[loop].to_location;
											taxi[cl_taxi[j].id].capacity = taxi[cl_taxi[j].id].capacity +1;
											l++;
											
											output[loop].revenue = city_detail[output[loop].from_location][output[loop].to_location];	
											taxi[output[loop].taxi_no].revenue = taxi[output[loop].taxi_no].revenue + city_detail[output[loop].from_location][output[loop].to_location];
											revenue = revenue + output[loop].revenue;
											//c3++;
											k++;
											break;
										}								
									}		
								}
							}
						}
						//c3++;
						break;
					}

					// Now we need to check the condition when our loop has reached the last position and there are cabs who have still not processed. 
					else if(i == (no_of_request - 1))
					{
						int location;
					// We will check for all the tax
						for (j=0; j<no_of_taxi; j++)
						{
						// If we have taxi whose capacity is greater than or equal to 1, means it has a person
							if(taxi[cl_taxi[j].id].capacity >= 1)
							{
								// The loop will run to that point and give the sorted result

								l = taxi[cl_taxi[j].id].capacity;
								for(k=taxi[cl_taxi[j].id].capacity; k>0; k--)
								{
									for(m=1; m<=l; m++)
									{
										a[m]=city_detail[location][t_service[cl_taxi[j].id][m].to_location];
										b[m]=m;
									}

									for(m=1; m<=l; m++)
									{
										for(n=m+1; n<=l; n++)
										{
												if(a[m]>a[n])
												{
									    				temp = a[m];
									    				a[m] = a[n];
									    				a[n] = temp;
													temp = b[m];
									    				b[m] = b[n];
									    				b[n] = temp;
												}
										}
									}
						
									// sort the location to get the closest location where it can go
							
									// Update time as time taken to reach the place
									taxi[cl_taxi[j].id].time = taxi[cl_taxi[j].id].time + a[1] * 2;

									// Update location as location of the place it has reached
									taxi[cl_taxi[j].id].location= t_service[cl_taxi[j].id][b[1]].to_location;

									location = t_service[cl_taxi[j].id][b[1]].to_location;
									// Decrease value of loop
									l--;

									// Update the array
									t_service[cl_taxi[j].id][b[1]].to_location = t_service[cl_taxi[j].id][l].to_location;						
						
									// Decrease the capacity
									taxi[cl_taxi[j].id].capacity = taxi[cl_taxi[j].id].capacity -1;
								}
							}
						}
						c3++;
						break;
					}
		
					else
						count++;
				}
				if(count == no_of_taxi)
				{
					// This counts the number of request that are not served
					c1++;
					count = 0;
				}
				count = 0;
			} 
		}
	

		printf("\n");
		for(i=0; i<no_of_taxi; i++)
		{
			printf("\nTaxi no : %d\n",(i+1));
			for(j=0; j<no_of_request; j++)
			{
				if(output[j].taxi_no == i && output[j].request_no != 0)
				{
					printf("%d  ",output[j].request_no);
				}
			}
			printf("\n\nThe revenue is %d\n",taxi[i].revenue);
			printf("\n");
		}

		printf("Total revenue obtained is %d\n",revenue);
		printf("\n");
	}
}

	

// Check the closest taxi to that location
void check_taxi(int location, int no_of_taxi, int time)
{
	// It will sort all the taxi according to the location which one is the closest
	int i,j,a_in[no_of_taxi],b_in[no_of_taxi],temp;
	for(i=0; i<no_of_taxi; i++)
	{
		a_in[i]=city_detail[taxi[i].location][location];
		b_in[i]= i;
	}
	
	for(i=0; i<no_of_taxi; i++)
	{
		for(j=i+1; j<no_of_taxi; j++)
		{
				if(a_in[i]>a_in[j])
				{
            				temp = a_in[i];
            				a_in[i] = a_in[j];
            				a_in[j] = temp;
					temp = b_in[i];
            				b_in[i] = b_in[j];
            				b_in[j] = temp;
				}
		}
	}
	
	// After sorting it updates the array
	for(i=0; i<no_of_taxi; i++)
	{
		cl_taxi[i].distance = a_in[i];
		cl_taxi[i].id = b_in[i];	
	}
}


void floydwarshall(int size)
{
	// It will calculate floyd warshall and give us the minimum distance from one location to another (all pair shortest path).
	int i,j,k,ans[max][max];

	for(i=0; i<size; i++)
	{
		for(j=0; j<size; j++)
		{
			if(i==j)
				ans[i][j]=0;
			else if(city_detail[i][j]==-1)
				ans[i][j]=1000;
			else
				ans[i][j]=city_detail[i][j];
		}
	}
	
	for(k=0; k<size; k++)
	{
		for(i=0; i<size; i++)
		{
			for(j=0; j<size; j++)
			{
				ans[i][j]=min((ans[i][k] + ans[k][j]), ans[i][j]);
			}
		}
	}

	for(i=0; i<size; i++)
	{
		for(j=0; j<size; j++)
		{
			city_detail[i][j]=ans[i][j];
		}
	}


}		

int min(int a, int b)
{
	if(a>b)
		return b;
	else
		return a;
}



