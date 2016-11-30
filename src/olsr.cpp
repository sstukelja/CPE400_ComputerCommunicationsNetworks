#include "../include/olsr.h"
#include <iostream>

using namespace std;

OLSR::OLSR()
{
	pushNodes(5);
    network[0]->addOneHopNeighbor(network[1]);
    network[0]->addOneHopNeighbor(network[3]);
    network[1]->addOneHopNeighbor(network[0]);
    network[1]->addOneHopNeighbor(network[2]);
    network[2]->addOneHopNeighbor(network[1]);
    network[2]->addOneHopNeighbor(network[3]);
    network[3]->addOneHopNeighbor(network[0]);
    network[3]->addOneHopNeighbor(network[2]);
    network[3]->addOneHopNeighbor(network[4]);
    network[4]->addOneHopNeighbor(network[3]);
	
    //routing table

}

//Deletes vector object and frees the memory allocated
OLSR::~OLSR()
{
	network.clear();
	network.shrink_to_fit();
}

//Adds nodes to the network
void OLSR::pushNodes(int num)
{
	network.resize(num);
    for(int i = 0; i < num; i++)
    {
        network[i] = new Node(i);
    }
}

//Returns the current number of nodes in the network
int OLSR::getNumOfNodes()
{
	return network.size();
}

//Sets up 2 Hop Neighbor Table
void OLSR::broadcastHello(Node* node)
{
    for(int i = 0; i < node->getOneHopNeighborNum(); i++)
    {
        Node *currentNeighbor = node -> getOneHopNeighbor(i);
        for(int j = 0; j < currentNeighbor -> getOneHopNeighborNum(); j++)
        {
			for(int k = 0; k < node -> getOneHopNeighborNum(); k++)
            {
            	if(node == currentNeighbor -> getOneHopNeighbor(j)) 
                {
					break;
                }
				else if((currentNeighbor -> getOneHopNeighbor(j) == node -> getOneHopNeighbor(k)))
				{
					break;
				}
				else if( node->inTwoHopTable(currentNeighbor -> getOneHopNeighbor(j)))
				{
					break;
				}
                else
				{
					node -> addTwoHopNeighbor(currentNeighbor -> getOneHopNeighbor(j));
					break;
				}
    		}
		}
	}
	
}

// Get Node
Node* OLSR::getNode(int index)
{
    return network[index];
}

void OLSR::topologyControl()
{
    for(int i = 0; i < getNumOfNodes(); i++)
    {
        if(network[i]->getOneHopNeighborNum() == 1)
        {
            network[i]->getOneHopNeighbor(0)->setMPR(true);
        }
    }

    int counter = 4;
    while(counter > 0)
    {
        for(int i = 0; i < network.size(); i++)
        {
            if(network[i]->getOneHopNeighborNum() == counter)
            {
                if(!(network[i]->neighboringMPR()))
                {
                    network[i]->setMPR(true);
                }
            }
        }
        counter--;
    }

}
/*
Route findRoute(Node* prev, Node* src, Node* dest)
{
	static Route routeBuild;
	static int sequence = 0;
	if(src->isOneHopNeighbor(dest))
	{
		routeBuild.setDestMPR(src);
		routeBuild.setDestAddress(dest);
		
	}
	else
	{
		for(int i = 0; i < src->getOneHopNeighborNum(); i++)
		{
			if(src->getOneHopNeighbor(i).getMPR())
			{
				findRoute(src->getOneHopNeighbor(i), dest);
			}
		}
	}
}

void OLSR::createRoutingTable(Node* node)
{
	for(int i = 0; i < network.size(); i++)
	{
		if(network[i] != node && !(node->isOneHopNeighbor(network[i])))
		{
			node->pushRoute(findRoute(NULL, node, network[i]));
		}
	}
}
*/
