// AgentPlus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "CSVParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

//CWinApp theApp;
using namespace std;
#define _MAX_NUMBER_OF_NODES 12000
#define _MAX_NUMBER_OF_LINKS 4000
#define _MAX_NUMBER_OF_ZONES 1000
#define _MAX_LABEL_COST 9999
#define _MAX_NUMBER_OF_OUTBOUND_NODES 10

int g_outbound_node_size[_MAX_NUMBER_OF_NODES];
int g_outbound_node_id[_MAX_NUMBER_OF_NODES][_MAX_NUMBER_OF_OUTBOUND_NODES];
int g_outbound_link_no[_MAX_NUMBER_OF_NODES][_MAX_NUMBER_OF_OUTBOUND_NODES];

int g_inbound_node_size[_MAX_NUMBER_OF_NODES];
int g_inbound_node_id[_MAX_NUMBER_OF_NODES][_MAX_NUMBER_OF_OUTBOUND_NODES];
int g_inbound_link_no[_MAX_NUMBER_OF_NODES][_MAX_NUMBER_OF_OUTBOUND_NODES];

int g_link_free_flow_travel_time[_MAX_NUMBER_OF_LINKS];
float g_link_link_length[_MAX_NUMBER_OF_LINKS];
int g_link_number_of_lanes[_MAX_NUMBER_OF_LINKS];
int g_link_mode_code[_MAX_NUMBER_OF_LINKS];
float g_link_capacity_per_time_interval[_MAX_NUMBER_OF_LINKS];
float g_link_speed[_MAX_NUMBER_OF_LINKS];

int g_number_of_links = 0;
int g_number_of_nodes = 0;
int g_number_of_zones = 0;
int g_maximum_node_number = 0;
int g_maximum_zone_number = 0;

int g_node_status_array[_MAX_NUMBER_OF_NODES];
float g_node_label_cost[_MAX_NUMBER_OF_NODES];
float g_node_predecessor[_MAX_NUMBER_OF_NODES];
float g_min_cost = _MAX_LABEL_COST;

int g_zone_no[_MAX_NUMBER_OF_ZONES];

int g_ListFront;
int g_ListTail;
int g_SENodeList[_MAX_NUMBER_OF_NODES];

int main()
{
	void g_ProgramStop();
	void SEList_clear();
	void SEList_push_front(int node);
	void SEList_push_back(int node);
	bool SEList_empty();
	int SEList_front();
	void SEList_pop_front();

	int node_id_flag[_MAX_NUMBER_OF_NODES] = { 0 };

	// initialization
	for (int i = 0; i < _MAX_NUMBER_OF_NODES; i++)
	{
		g_outbound_node_size[i] = 0;
		g_inbound_node_size[i] = 0;
	}

	CCSVParser parser;
	if (parser.OpenCSVFile("input_node.csv", false))
	{
		g_number_of_nodes = 0;

		std::map<int, int> node_id_map;

		parser.m_bDataHubSingleCSVFile = true;

		while (parser.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
		{
			if (parser.m_DataHubSectionName.find("Node") != string::npos)
			{
				string name;

				int node_type;
				int node_id;
				double X;
				double Y;

				if (parser.GetValueByFieldName("node_id", node_id) == false)
					continue;

				if (node_id > g_maximum_node_number)
					g_maximum_node_number = node_id;

				if (node_id <= 0 || g_maximum_node_number >= _MAX_NUMBER_OF_NODES)
				{
					cout << "node_id " << node_id << " is out of range" << endl;
					g_ProgramStop();
				}
				parser.GetValueByFieldName("node_type", node_type);
				parser.GetValueByFieldName("x", X);
				parser.GetValueByFieldName("y", Y);

				node_id_flag[node_id] = 1;

				g_number_of_nodes++;
			}
		}
		/*cout << g_number_of_nodes << endl;*/
		parser.CloseCSVFile();
	}

	if (parser.OpenCSVFile("input_activity_location.csv", false))
	{

		/*std::map<int, int> node_id_map;*/

		parser.m_bDataHubSingleCSVFile = true;

		while (parser.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
		{
			if (parser.m_DataHubSectionName.find("zone") != string::npos)
			{

				int zone_id;
				int node_id;
				int external_OD_flag;

				if (parser.GetValueByFieldName("zone_id", zone_id) == false)
					continue;
				/*cout << "1" << endl;*/
				if (zone_id > g_maximum_zone_number)
					g_maximum_zone_number = zone_id;

				if (zone_id <= 0 || g_maximum_zone_number >= _MAX_NUMBER_OF_NODES)
				{
					cout << "zone_id " << zone_id << " is out of range" << endl;
					g_ProgramStop();
				}
				parser.GetValueByFieldName("node_id", node_id);
				parser.GetValueByFieldName("external_OD_flag", external_OD_flag);

				g_zone_no[zone_id] = node_id;
				/*	cout << g_zone_no[zone_id] << endl;*/

				g_number_of_zones++;

			}

		}
		/*cout << g_number_of_zones << endl;*/
		parser.CloseCSVFile();
	}



	if (parser.OpenCSVFile("input_link.csv", false))
	{
		g_number_of_links = 0;
		/*std::map<int, int> node_id_map;*/

		parser.m_bDataHubSingleCSVFile = true;
		while (parser.ReadRecord())
		{
			if (parser.m_DataHubSectionName.find("Link") != string::npos)
			{
				int from_node_id = 0;
				int to_node_id = 0;

				if (parser.GetValueByFieldName("from_node_id", from_node_id) == false)
					continue;
				if (parser.GetValueByFieldName("to_node_id", to_node_id) == false)
					continue;

				if (from_node_id <= 0 || from_node_id >= _MAX_NUMBER_OF_NODES)
				{
					cout << "from_node_id " << from_node_id << " is out of range" << endl;
					g_ProgramStop();
				}

				if (to_node_id <= 0 || to_node_id >= _MAX_NUMBER_OF_NODES)
				{
					cout << "to_node_id " << to_node_id << " is out of range" << endl;
					g_ProgramStop();
				}

				if (node_id_flag[from_node_id] != 1)
				{
					cout << "from_node_id " << from_node_id << " has not been defined in node block" << endl;
					g_ProgramStop();
				}

				if (node_id_flag[to_node_id] != 1)
				{
					cout << "to_node_id " << to_node_id << " has not been defined in node block" << endl;
					g_ProgramStop();
				}

				// add the to node id into the outbound (adjacent) node list

				int direction = 1;
				parser.GetValueByFieldName("direction", direction);

				if (direction <= -2 || direction >= 2)
				{
					cout << "direction " << direction << " is out of range" << endl;
					g_ProgramStop();
				}

				for (int link_direction = -1; link_direction <= 1; link_direction += 2)  // called twice; -1 direction , 1 direction 
				{
					if (direction == -1 && link_direction == 1)
						continue; // skip

					if (direction == 1 && link_direction == -1)
						continue; // skip

					// then if  direction == 0 or 2 then create the corresponding link

					int directional_from_node_id = from_node_id;
					int directional_to_node_id = to_node_id;

					if (link_direction == -1) // reverse direction;
					{
						directional_from_node_id = to_node_id;
						directional_to_node_id = from_node_id;
					}

					g_outbound_node_id[directional_from_node_id][g_outbound_node_size[directional_from_node_id]] = directional_to_node_id;
					g_outbound_link_no[directional_from_node_id][g_outbound_node_size[directional_from_node_id]] = g_number_of_links;
					g_outbound_node_size[directional_from_node_id]++;

					g_inbound_node_id[directional_to_node_id][g_inbound_node_size[directional_to_node_id]] = directional_from_node_id;
					g_inbound_link_no[directional_to_node_id][g_inbound_node_size[directional_to_node_id]] = g_number_of_links;
					g_inbound_node_size[directional_to_node_id]++;

					float link_length = 1;
					int number_of_lanes = 1;
					int mode_code = 0;
					float capacity_per_time_interval = 1;
					int travel_time = 1;
					float speed = 1;

					parser.GetValueByFieldName("length", link_length);
					parser.GetValueByFieldName("number_of_lanes", number_of_lanes);
					parser.GetValueByFieldName("mode_code", mode_code);
					parser.GetValueByFieldName("lane_capacity_in_vhc_per_hour", capacity_per_time_interval);
					/*parser.GetValueByFieldName("travel_time", travel_time);*/
					parser.GetValueByFieldName("speed_limit", speed);

					g_link_free_flow_travel_time[g_number_of_links] = travel_time;
					g_link_link_length[g_number_of_links] = link_length;
					g_link_number_of_lanes[g_number_of_links] = number_of_lanes;
					g_link_mode_code[g_number_of_links] = mode_code;
					g_link_capacity_per_time_interval[g_number_of_links] = capacity_per_time_interval;
					g_link_speed[g_number_of_links] = speed;

					// increase the link counter by 1
					g_number_of_links++;

				}
			}
		}
		/*	cout << g_number_of_links << endl;*/
		parser.CloseCSVFile();
	}

	for (int i = 1; i <= g_maximum_zone_number; i++)
	{

		/*int origin_node = 1357;*/
		int origin_node[_MAX_NUMBER_OF_ZONES];

		origin_node[i] = g_zone_no[i];


		int path_node_sequence[_MAX_NUMBER_OF_NODES];
		int path_number_of_nodes;
		//float arc_cost[_MAX_NUMBER_OF_LINKS];// calculate it!!



		if (g_outbound_node_size[origin_node[i]] == 0)
		{
			return _MAX_LABEL_COST;
		}

		for (int k = 1; k <= g_maximum_node_number; k++) //Initialization for all nodes
		{
			g_node_status_array[k] = 0;  // not scanned
			g_node_label_cost[k] = _MAX_LABEL_COST;
			g_node_predecessor[k] = -1;  // pointer to previous NODE INDEX from the current label at current node and time
		}

		g_node_label_cost[origin_node[i]] = 0; //Initialization for origin node, cost = 0


		SEList_clear();
		SEList_push_front(origin_node[i]);

		while (!SEList_empty())
		{
			int from_node = SEList_front();//pop a node FromID for scanning

			SEList_pop_front();  // remove current node FromID from the SE list

			/*	TRACE("SE node: %d\n", from_node);*/

			g_node_status_array[from_node] = 2;

			//scan all outbound nodes of the current node
			for (int m = 0; m < g_outbound_node_size[from_node]; m++)  // for each link (i,j) belong A(i)
			{
				int link_no = g_outbound_link_no[from_node][m];
				int to_node = g_outbound_node_id[from_node][m];

				if (g_node_label_cost[from_node] < _MAX_LABEL_COST - 1)  // for feasible time-space point only
				{
					float temporary_label_cost = g_node_label_cost[from_node] + g_link_link_length[link_no];// arc_cost should be defined when reading the link file!
					if (temporary_label_cost < g_node_label_cost[to_node]) // we only compare cost at the downstream node ToID at the new arrival time t
					{
						// update cost label and node/time predecessor
						g_node_label_cost[to_node] = temporary_label_cost;
						g_node_predecessor[to_node] = from_node;  // pointer to previous physical NODE INDEX from the current label at current node and time

						if (g_node_status_array[to_node] == 2) // in the SEList_TD before
						{
							SEList_push_front(to_node);
							/*SEList_pop_front();*/
							g_node_status_array[to_node] = 1;
						}
						if (g_node_status_array[to_node] == 0)  // not be reached
						{
							SEList_push_back(to_node);
							g_node_status_array[to_node] = 1;
						}
						
					}
				}				
			}
			
		}
		cout << g_node_status_array[origin_node[i]] << endl;
		
		/*int destination_node = 1352;*/
		int destination_node[_MAX_NUMBER_OF_ZONES];

		for (int j = 1; j <= g_maximum_zone_number; j++)	
		{
			float g_min_cost = _MAX_LABEL_COST;

			destination_node[j] = g_zone_no[j];

			/*cout << destination_node[j] << endl;*/

			if (g_node_label_cost[destination_node[j]] < g_min_cost)
			{
				g_min_cost = g_node_label_cost[destination_node[j]];
			}

			int reversed_path_node_sequence[_MAX_NUMBER_OF_NODES];

			// backtrack to the origin (based on node and time predecessors)
			int	node_size = 0;
			reversed_path_node_sequence[node_size] = destination_node[j];//record the first node backward, destination node
			
			node_size++;

			int pred_node = g_node_predecessor[destination_node[j]];

			while (pred_node != -1 && node_size < _MAX_NUMBER_OF_NODES) // scan backward in the predessor array of the shortest path calculation results
			{
				reversed_path_node_sequence[node_size] = pred_node;
				node_size++;

				//record current values of node and time predecessors, and update PredNode and PredTime
				int pred_node_record = pred_node;
				pred_node = g_node_predecessor[pred_node_record];
			}

			cout << "The node sequence of the shorest path from " << origin_node[i] << " to " << destination_node[j] << " is";

			for (int n = 0; n < node_size; n++)
			{
				path_node_sequence[n] = reversed_path_node_sequence[node_size - n - 1];
				cout << ", " << path_node_sequence[n];
			}

			cout << endl;
			cout << "Its minimal cost is: " << g_min_cost << endl;
			cout << endl;

			path_number_of_nodes = node_size;
			/*cout << path_number_of_nodes << endl;*/
			/*return total_cost;*/
		}

	}

	
}

// SEList: Scan List implementation: the reason for not using STL-like template is to avoid overhead associated pointer allocation/deallocation
void SEList_clear()
{
	g_ListFront = -1;
	g_ListTail = -1;
}

void SEList_push_front(int node)
{
	if (g_ListFront == -1)  // start from empty
	{
		g_SENodeList[node] = -1;
		g_ListFront = node;
		g_ListTail = node;
	}
	else
	{
		g_SENodeList[node] = g_ListFront;
		g_ListFront = node;
	}

}

void SEList_push_back(int node)
{
	if (g_ListFront == -1)  // start from empty
	{
		g_ListFront = node;
		g_ListTail = node;
		g_SENodeList[node] = -1;
	}
	else
	{
		g_SENodeList[g_ListTail] = node;
		g_SENodeList[node] = -1;
		g_ListTail = node;
	}
}

bool SEList_empty()
{
	return(g_ListFront == -1);
}

int SEList_front()
{
	return g_ListFront;
}

void SEList_pop_front()
{
	int tempFront = g_ListFront;
	g_ListFront = g_SENodeList[g_ListFront];
	g_SENodeList[tempFront] = -1;
}

void g_ProgramStop()
{
	cout << "Exit program " << endl;
	exit(0);
}
