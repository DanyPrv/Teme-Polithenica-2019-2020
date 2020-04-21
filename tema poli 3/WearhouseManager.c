#include <stdio.h>
#include "WearhouseManager.h"


Package *create_package(long priority, const char* destination){
	// TODO 3.1
	Package *package = (Package*) malloc(sizeof(Package));
	package->priority=priority;
	if(destination == NULL){
		package->destination = NULL;
	}
	else{
		package->destination= (char*)malloc(100*sizeof(char));
		strcpy(package->destination,destination);
	}
	return package;
}

void destroy_package(Package* package){
	// TODO: 3.1
	if(package != NULL){
		free(package->destination);
		free(package);
	}
 	else free(package);
}

Manifest* create_manifest_node(void){
	// TODO: 3.1
	Manifest *manifest_node=(Manifest*)malloc(sizeof(Manifest));
	manifest_node->package=NULL;
	manifest_node->next=NULL;
	manifest_node->prev=NULL;
	return manifest_node;
}

void destroy_manifest_node(Manifest* manifest_node){
	// TODO: 3.1.
	destroy_package(manifest_node->package);
	free(manifest_node);

}

Wearhouse* create_wearhouse(long capacity){
	// TODO: 3.2
	if(capacity != 0)
	{
		Wearhouse *wearhouse=(Wearhouse*)malloc(sizeof(Wearhouse));
		wearhouse->packages=(Package**)malloc(sizeof(Package*));	
		wearhouse->capacity=capacity;
		wearhouse->size=0;
		return wearhouse;	
	}
	else
		return NULL;
}

Wearhouse *open_wearhouse(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w){
	if(w->size != 0)
		return 0;
	else return 1;		

}

long wearhouse_is_full(Wearhouse *w){
	// TODO: 3.2
	if(w->size == w->capacity)
		return 1;
	else return 0;
}

long wearhouse_max_package_priority(Wearhouse *w){
	// TODO: 3.2
	int max=0;
	for(long i=0; i<w->size; i++){
		if((*(w->packages[i])).priority>max){
			max=(*(w->packages[i])).priority;
		}
	}
	return max;
}

long wearhouse_min_package_priority(Wearhouse *w){
	long min=wearhouse_max_package_priority(w);
	for(long i=0; i<w->size; i++){
		if((*(w->packages[i])).priority<min){
			min=(*(w->packages[i])).priority;
		}
	}
	return min;
}


void wearhouse_print_packages_info(Wearhouse *w){
	for(long i = 0; i < w->size; i++){
		printf("P: %ld %s\n",
				w->packages[i]->priority,
				w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse){
	// TODO: 3.2
//	int i;
//	for(i=0; i<wearhouse->size; i++){
//		destroy_package(wearhouse->packages[i]);
//		}

}



Robot* create_robot(long capacity){
		// TODO: 3.2
	Robot *robot=(Robot*)malloc(sizeof(Robot));	
	robot->manifest=NULL;
	robot->capacity=capacity;
	robot->next=NULL;

		return robot;
}

int robot_is_full(Robot* robot){
	// TODO: 3.2
	if(robot->size == robot->capacity)
		return 1;
	else return 0;
}

int robot_is_empty(Robot* robot){
	// TODO: 3.2
	if(robot->size == 0)
		return 1;
	else return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority){
	// TODO: 3.2
	int i;
	Package *first=NULL;
	for(i =0; i<w->size; i++){
		if(w->packages[i]->priority == priority){
			first=w->packages[i];
			break;
		}
	}
	if(first != NULL)
		return first;
	else return NULL;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package){
	// TODO: 3.2
	int i, pos;
	for(i=0; i<w->size; i++){
		if(w->packages[i] == package){
			pos=i;
			while(pos<w->size-1){
				w->packages[pos]=w->packages[pos+1];
				pos++;
			}
			break;
		}
	}
	w->size--;
}

void robot_load_one_package(Robot* robot, Package* package){
	// TODO:  3.2

}

long robot_load_packages(Wearhouse* wearhouse, Robot* robot){
	// TODO: 3.2
	return 0;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination){
	// TODO: 3.2

	return NULL;
}

void destroy_robot(Robot* robot){
	// TODO: 3.2
}

void robot_unload_packages(Truck* truck, Robot* robot){
	// TODO: 3.3
}



// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot *parkinglot){
	int found_truck = 0;
	long size = 0;
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while(m_iterator != NULL){
		while(arrived_iterator != parkinglot->arrived_trucks){
			size  = truck_destination_robots_unloading_size(arrived_iterator);
			if(strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
					size < (arrived_iterator->capacity-arrived_iterator->size)){
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if(found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if(found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while(r_iterator != NULL){
		Package *pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if(m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if(prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot){
	Manifest *iterator = robot->manifest;
	while(iterator != NULL){
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}



Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time){
	// TODO: 3.3
	Truck *truck= (Truck*)malloc(sizeof(Truck));
	if(destination == NULL){
		truck->destination = NULL;
	}
	else{
		truck->destination=(char*)malloc(100*sizeof(char));
		strcpy(truck->destination, destination);
	}
	truck->next=NULL;
	truck->size=0;
	truck->capacity=capacity;
	truck->in_transit_time=0;
	truck->transit_end_time=transit_time;
	truck->departure_time=departure_time;
	truck->manifest=NULL;
	truck->unloading_robots=NULL;
	return truck;
}

int truck_is_full(Truck *truck){
	if(truck->capacity == truck->size)
		return 1;
	else return 0;
}

int truck_is_empty(Truck *truck){
	if(truck->size == 0)
		return 1;
	else return 0;
}

long truck_destination_robots_unloading_size(Truck* truck){
	// TODO: 3.3
	return 0;
}


void truck_print_info(Truck* truck){
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
			truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while(m_iterator != NULL){
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while(r_iterator != NULL){
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}


void destroy_truck(Truck* truck){
	// TODO: 3.3
	free(truck->destination);
	free(truck);
}


Parkinglot* create_parkinglot(void){
	// TODO: 3.4
	// Allocate parking lot
	Parkinglot* lot=(Parkinglot*)malloc(sizeof(Parkinglot));
	lot->arrived_trucks=NULL;
	lot->departed_trucks=NULL;
	lot->pending_robots=NULL;
	lot->standby_robots=NULL;

	return lot;
}

Parkinglot* open_parckinglot(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if(token[0] == 'T'){
			token = strtok(NULL, ",\n ");
			char *destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck *truck = create_truck(destination, capacity, transit_time, departure_time);

			if(arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}else if(token[0] == 'R'){
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot *robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot){
	// TODO: 3.4
}

void parkinglot_remove_robot(Parkinglot *parkinglot, Robot* robot){
	// TODO: 3.4
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->pending_robots == NULL)
		return 1;
	else return 0;
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot){
	// TODO: 3.4
	return 0;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->departed_trucks != NULL)
	return 0;
	else return 1;
}


void destroy_parkinglot(Parkinglot* parkinglot){
	// TODO: 3.4
}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->arrived_trucks->next;
	while(iterator != parkinglot->arrived_trucks){

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->departed_trucks->next;
	while(iterator != parkinglot->departed_trucks){
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->standby_robots->next;
	while(iterator != parkinglot->standby_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}


void truck_departed(Parkinglot *parkinglot, Truck* truck){
	// TODO: 3.5
	// Search through arrived list, if exists node is found remove it
	// Note: this must remove the node from the list, NOT deallocate it
}


void truck_arrived(Parkinglot *parkinglot, Truck* truck){
	if(parkinglot == NULL || truck == NULL) return;

	// TODO: 3.5
	// Search through departed list, if exists node is found remove it
	// Note: this must remove the node not deallocate it

}

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck){
	// TODO:  3.5
//	Robot* r->iterator = truck->unloading_robots;
//	while(r_iterator != NULL){
//		Robot* aux = r_iterator;
//		r_iterator=r_iterator->next;
//		parkinglot_add_robot(parkinglot, aux);
//	}
//	truck->unloading_robots = NULL;

}


// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour){
	// TODO: 3.5
}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot){
	// TODO: 3.5
}

void robot_swarm_collect(Wearhouse *wearhouse, Parkinglot *parkinglot){
	Robot *head_robot = parkinglot->standby_robots;
	Robot *current_robot = parkinglot->standby_robots->next;
	while(current_robot != parkinglot->standby_robots){

		// Load packages from wearhouse if possible
		if(!robot_load_packages(wearhouse, current_robot)){
			break;
		}

		// Remove robot from standby list
		Robot *aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot *parkinglot){

	Robot *current_robot = parkinglot->pending_robots->next;

	while(current_robot != parkinglot->pending_robots){
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if(!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot){
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	while(arrived_iterator != parkinglot->arrived_trucks){
		Robot *current_robot = arrived_iterator->unloading_robots;
		while(current_robot != NULL){
			robot_unload_packages(arrived_iterator, current_robot);
			Robot *aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}

