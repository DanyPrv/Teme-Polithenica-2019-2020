#define _GNU_SOURCE
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "WearhouseManager.h"


Package *create_package(long priority, const char* destination){
	Package *package = (Package*) malloc(sizeof(Package));
	package->priority=priority;
	if(destination == NULL){
		package->destination = NULL;
	}
	else{
		package->destination= (char*)malloc(sizeof(strlen(destination))+1);
		strcpy(package->destination,destination);
        package->destination[strlen(destination)]='\0';
	}
	return package;
}

void destroy_package(Package* package){
	// TODO: 3.1
	if(package!=NULL){
        if(package->destination != NULL)
            free(package->destination);
        free(package);
	}
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
    if(manifest_node!=NULL){
        destroy_package(manifest_node->package);
        free(manifest_node);
    }
}

Wearhouse* create_wearhouse(long capacity){
	// TODO: 3.2
	if(capacity<=0)
        return NULL;
    Wearhouse *wearhouse=(Wearhouse*)malloc(sizeof(Wearhouse));
    wearhouse->packages=(Package**)malloc(capacity*sizeof(Package*));
    wearhouse->capacity=capacity;
    wearhouse->size=0;
    return wearhouse;
}

Wearhouse *open_wearhouse(const char* file_path){
	char* line = (char*)malloc(100);
	size_t len = 100;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((fgets(line, len, fp)) != NULL){
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		//free(line);
		//line = NULL;
		//len = 0;
	}

	while(fgets(line, len, fp) != NULL){
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		//free(line);
		//line = NULL;
		//len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w){
	// TODO: 3.2
    if(w->size != 0)
		return 0;
    return 1;
}

long wearhouse_is_full(Wearhouse *w){
	// TODO: 3.2
	if(w->size == w->capacity)
		return 1;
    return 0;
}

long wearhouse_max_package_priority(Wearhouse *w){
	// TODO: 3.2
    long Max=-1;
	for(long i=0; i<w->size; i++)
		if(w->packages[i]->priority >Max)
			Max=w->packages[i]->priority;
	return Max;
}

long wearhouse_min_package_priority(Wearhouse *w){
	// TODO: 3.2
    long Min=wearhouse_max_package_priority(w);
	for(long i=0; i<w->size; i++)
		if(w->packages[i]->priority<Min)
			Min=w->packages[i]->priority;
	return Min;
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
	if(wearhouse!=NULL){
        long i;
        for(i=0;i<wearhouse->size;i++)
            destroy_package(wearhouse->packages[i]);
        free(wearhouse->packages);
        free(wearhouse);
	}
}


Robot* create_robot(long capacity){
	// TODO: 3.2
    Robot *robot=(Robot*)malloc(sizeof(Robot));
	robot->manifest=NULL;
	robot->capacity=capacity;
	robot->size=0;
	robot->next=NULL;
    return robot;
}

int robot_is_full(Robot* robot){
	// TODO: 3.2
	if(robot->size == robot->capacity)
		return 1;
    return 0;
}

int robot_is_empty(Robot* robot){
	// TODO: 3.2
	if(robot->size == 0)
		return 1;
	return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority){
	// TODO: 3.2
    long i;
	for(i=0; i<w->size; i++)
		if(w->packages[i]->priority == priority)
			return w->packages[i];
	return NULL;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package){
	// TODO: 3.2
	long i=0;
	while(i<w->size&&(w->packages[i])!=package)
        i++;
    for(;i<w->size-1;i++)
        w->packages[i]=w->packages[i+1];
    w->size--;
}

void robot_load_one_package(Robot* robot, Package* package){
	// TODO:  3.2
    if(robot->size==0){
        robot->manifest=create_manifest_node();
        robot->manifest->package=package;
        robot->size++;
        return;
    }
    Manifest* itr = robot->manifest;
    //daca trebuie adaugat primul
    if(itr->package->priority<package->priority ||
       (itr->package->priority==package->priority && strcmp(itr->package->destination,package->destination)>0)){
        Manifest* manifest = create_manifest_node();
        manifest->package=package;
        manifest->next=itr;
        itr->prev=manifest;
        robot->manifest=manifest;
        robot->size++;
        return;
    }

    Manifest* node = create_manifest_node();
    node->package=package;
    Manifest* prev= itr;
    while (itr!=NULL){
        if(itr->package->priority<package->priority){
            node->next=itr;
            node->prev=itr->prev;
            itr->prev=node;
            prev->next=node;
            robot->size++;
            return;
        }
        else if(itr->package->priority==package->priority &&
               strcmp(package->destination,itr->package->destination)<0){
            node->next=itr;
            node->prev=itr->prev;
            itr->prev=node;
            prev->next=node;
            robot->size++;
            return;
        }
        prev=itr;
        itr=itr->next;
    }
    prev->next=node;
    node->prev=prev;
    robot->size++;
}

long robot_load_packages(Wearhouse* wearhouse, Robot* robot){
	// TODO: 3.2
	int initialRobotSize = robot->size;
	while(robot->size<robot->capacity&&wearhouse_is_empty(wearhouse)==0){

	    long MaxPr = wearhouse_max_package_priority(wearhouse);
	    Package *pkg = robot_get_wearhouse_priority_package(wearhouse,MaxPr);

        robot_load_one_package(robot,pkg);

        robot_remove_wearhouse_package(wearhouse,pkg);
    }

	return robot->size-initialRobotSize;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination){
	// TODO: 3.2
    Manifest* itr = robot->manifest;
    while(itr!=NULL){
        if(strcmp(itr->package->destination,destination)==0)
            return itr->package;
        itr=itr->next;
    }
	return NULL;
}

void destroy_robot(Robot* robot){
	// TODO: 3.2
	if(robot!=NULL){
        Manifest* itr = robot->manifest;
        while(itr!=NULL){
            Manifest* aux=itr;
            itr=itr->next;
            destroy_manifest_node(aux);
        }
        free(robot);
	}
}

void robot_unload_packages(Truck* truck, Robot* robot){
	// TODO: 3.3
    Manifest* itr = robot->manifest;
    while(itr!=NULL && !truck_is_full(truck)){
        if(strcmp(truck->destination,itr->package->destination)==0){
            Manifest* aux = itr->next;
            //eliminam din lista robotului
            if(itr->prev==NULL){
                //e primul element
                robot->manifest=robot->manifest->next;
                robot->manifest->prev=NULL;
            }
            else{
                itr->prev->next=itr->next;
                itr->next->prev=itr->prev;
            }


            //adaugam in lista truckului
            itr->next=truck->manifest;
            if(truck->manifest!=NULL){
                truck->manifest->prev=itr;
            }
            truck->manifest=itr;


            robot->size--;
            truck->size++;
            itr=aux;
        }
        else
            itr=itr->next;
    }
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
	// TODO: 3.3
	if(truck->capacity==truck->size)
        return 1;
	return 0;
}

int truck_is_empty(Truck *truck){
	// TODO: 3.3
	if(truck->size==0)
        return 1;
    return 0;
}

long truck_destination_robots_unloading_size(Truck* truck){
	// TODO: 3.3
	long total_unloading_size=0;
	Robot* rbt = truck->unloading_robots;
	while(rbt!=NULL){
        Manifest* pkg = rbt->manifest;
        while(pkg!=NULL){
            if(strcmp(pkg->package->destination,truck->destination)==0){
                total_unloading_size+=rbt->size;
                rbt=rbt->next;
                break;
            }
            pkg=pkg->next;
        }
        rbt=rbt->next;
	}
	return total_unloading_size;
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
	if(truck!=NULL){
        free(truck->destination);
        Manifest* manifest = truck->manifest;
        while(manifest!=NULL){
            Manifest* aux = manifest;
            manifest=manifest->next;
            destroy_manifest_node(aux);
        }
        Robot* rbt = truck->unloading_robots;
        while(rbt!=NULL){
            Robot*aux = rbt;
            rbt = rbt->next;
            destroy_robot(aux);
        }
        free(truck);
	}
}


Parkinglot* create_parkinglot(void){
	// TODO: 3.4
	Parkinglot* pkglot=(Parkinglot*)malloc(sizeof(Parkinglot));

	pkglot->arrived_trucks=(Truck*)malloc(sizeof(Truck));
	pkglot->arrived_trucks->next=pkglot->arrived_trucks;

	pkglot->departed_trucks=(Truck*)malloc(sizeof(Truck));
	pkglot->departed_trucks->next=pkglot->departed_trucks;

	pkglot->pending_robots=(Robot*)malloc(sizeof(Robot));
	pkglot->pending_robots->next=pkglot->pending_robots;

	pkglot->standby_robots=(Robot*)malloc(sizeof(Robot));
	pkglot->standby_robots->next=pkglot->standby_robots;

	return pkglot;
}

Parkinglot* open_parckinglot(const char* file_path){
	char* line = (char*)malloc(100);
	size_t len = 100;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while(fgets(line, len, fp) != NULL){
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

		//free(line);
		//line = NULL;
		//len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot){
	if(robot->size==0){
        //add in stand by
        Robot* prev = parkinglot->standby_robots;
        Robot* itr=parkinglot->standby_robots->next;
        while(itr!=parkinglot->standby_robots&&itr->capacity>robot->capacity){
                prev=itr;
                itr=itr->next;
        }
        robot->next=prev->next;
        prev->next=robot;
        return;
	}
        Robot* prev = parkinglot->pending_robots;
        Robot* itr=parkinglot->pending_robots->next;
        while(itr!=parkinglot->pending_robots&&itr->capacity>robot->capacity){
                prev=itr;
                itr=itr->next;
        }
        robot->next=prev->next;
        prev->next=robot;
}

void parkinglot_remove_robot(Parkinglot *parkinglot, Robot* robot){
	// TODO: 3.4
	Robot* itr = parkinglot->pending_robots;
	Robot* prev;
	if(itr!=NULL){
        if(itr==robot){
            if(itr->next==itr){
                parkinglot->pending_robots=NULL;
                return;
            }
            Robot* itr2=itr;
            while(itr2->next!=itr)
                itr2=itr2->next;
            itr2->next=itr->next;
            parkinglot->pending_robots=itr->next;
            return;
        }
        prev=itr;
        itr=itr->next;
        while(itr!=parkinglot->pending_robots){
            if(itr==robot){
                prev->next=itr->next;
                return;
            }
            prev=itr;
            itr=itr->next;
        }
	}


	itr = parkinglot->standby_robots;
	if(itr!=NULL){
        if(itr==robot){
            if(itr->next==itr){
                parkinglot->standby_robots=NULL;
                return;
            }
            Robot* itr2=itr;
            while(itr2->next!=itr)
                itr2=itr2->next;
            itr2->next=itr->next;
            parkinglot->standby_robots=itr->next;
            return;
        }
        prev=itr;
        itr=itr->next;
        while(itr!=parkinglot->standby_robots){
            if(itr==robot){
                prev->next=itr->next;
                return;
            }
            prev=itr;
            itr=itr->next;
        }
	}
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->pending_robots->next!=parkinglot->pending_robots)
        return 1;
	return 0;
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->arrived_trucks->next==parkinglot->arrived_trucks){
       return 0;
	}
	Truck* truckPtr=parkinglot->arrived_trucks->next;
	while(truckPtr!=parkinglot->arrived_trucks){
        if(truckPtr->size!=0)
            return 0;
        truckPtr=truckPtr->next;
	}
	return 1;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->departed_trucks->next==parkinglot->departed_trucks){
       return 0;
	}
	Truck* truckPtr=parkinglot->departed_trucks->next;
	while(truckPtr!=parkinglot->departed_trucks){
        if(truckPtr->size!=0)
            return 0;
        truckPtr=truckPtr->next;
	}
	return 1;
}


void destroy_parkinglot(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->arrived_trucks!=NULL){
        Truck* itr=parkinglot->arrived_trucks->next;
        while(itr!=parkinglot->arrived_trucks){
            Truck* aux=itr;
            itr=itr->next;
            destroy_truck(aux);
        }
        free(parkinglot->arrived_trucks);
	}
    if(parkinglot->departed_trucks!=NULL){
        Truck* itr=parkinglot->departed_trucks->next;
        while(itr!=parkinglot->departed_trucks){
            Truck* aux=itr;
            itr=itr->next;
            destroy_truck(aux);
        }
        free(parkinglot->departed_trucks);
	}
    if(parkinglot->pending_robots!=NULL){
        Robot* itr=parkinglot->pending_robots->next;
        while(itr!=parkinglot->pending_robots){
            Robot* aux=itr;
            itr=itr->next;
            destroy_robot(aux);
        }
        free(parkinglot->pending_robots);
	}
	if(parkinglot->standby_robots!=NULL){
        Robot* itr=parkinglot->standby_robots->next;
        while(itr!=parkinglot->standby_robots){
            Robot* aux=itr;
            itr=itr->next;
            destroy_robot(aux);
        }
        free(parkinglot->standby_robots);
	}
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
    if(parkinglot->pending_robots==NULL)return;
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot){
    if(parkinglot->standby_robots==NULL)
        return;
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
	// Note: this must remove the node from the list, NOT deallocate it
    if(parkinglot == NULL || truck == NULL) return;

    //eliminare din lista de arrived
    Truck* prev = parkinglot->arrived_trucks;
    Truck* ptr = parkinglot->arrived_trucks->next;
    while(ptr!=parkinglot->arrived_trucks){
        if(ptr==truck){
            prev->next=ptr->next;
            break;
        }
        prev=ptr;
        ptr=ptr->next;
    }

    prev = parkinglot->departed_trucks;
    ptr = parkinglot->departed_trucks->next;
    while(ptr!=parkinglot->departed_trucks&&truck->departure_time>ptr->departure_time){
        prev=ptr;
        ptr=ptr->next;
    }
    truck->next=ptr;
    prev->next=truck;

}


void truck_arrived(Parkinglot *parkinglot, Truck* truck){
	if(parkinglot == NULL || truck == NULL) return;
	// TODO: 3.5
	//eliminare din departed
    Truck* prev = parkinglot->departed_trucks;
    Truck* ptr = parkinglot->departed_trucks->next;
    while(ptr!=parkinglot->departed_trucks){
        if(ptr==truck){
            prev->next=ptr->next;
            break;
        }
        prev=ptr;
        ptr=ptr->next;
    }

	//il adauga in arrived
    prev = parkinglot->arrived_trucks;
    ptr = parkinglot->arrived_trucks->next;
    while(ptr!=parkinglot->arrived_trucks){
        if(strcmp(truck->destination,ptr->destination)<0)
            break;
        else if(strcmp(truck->destination,ptr->destination)==0&&truck->departure_time<ptr->departure_time)
            break;
        prev=ptr;
        ptr=ptr->next;
    }
    truck->next=ptr;
    prev->next=truck;

    Manifest* manifest=truck->manifest;
    while(manifest!=NULL){
        Manifest* aux = manifest;
        manifest=manifest->next;
        //destroy_manifest_node(aux);
    }
    truck->size=0;
    truck->transit_end_time=0;
    truck->manifest=NULL;
	// Search through departed list, if exists node is found remove it
	// Note: this must remove the node not deallocate it

}

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck){
	// TODO:  3.5
    Robot* itr = truck->unloading_robots;
    while(itr!=NULL){
        Robot* aux = itr->next;
        parkinglot_add_robot(parkinglot,itr);
        itr=aux;
    }
    truck->unloading_robots=NULL;
}


// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour){
	// TODO: 3.5
	Truck* itr = parkinglot->arrived_trucks->next;
	while(itr!=parkinglot->arrived_trucks){
        Truck* aux = itr->next;
        if(day_hour==itr->departure_time){
            truck_transfer_unloading_robots(parkinglot,itr);
            truck_departed(parkinglot,itr);
        }
        itr=aux;
	}
}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot){
	// TODO: 3.5
	Truck*itr = parkinglot->departed_trucks->next;
	while(itr!=parkinglot->departed_trucks){
        Truck* aux = itr->next;
        itr->in_transit_time++;
        if(itr->in_transit_time==itr->transit_end_time){
            itr->in_transit_time=0;
            truck_arrived(parkinglot,itr);
        }
        itr=aux;
	}
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

