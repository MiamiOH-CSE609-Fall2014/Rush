/*	Author: Andrew Rush
Class: ECE 561
Date: 10/24/2014
Project: Project2  */
#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/dijkstra.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <random>
#include <tuple>

//Default Namespace:
using namespace lemon;
using namespace std;

//Simulation Parameters:
static int state = 1; // 1 = variable route/destination w/ wavelength contiguity constraint
static int numberWavelengths = 6;
static int numberNodes = 28;
static int numberSystemLinks = 64;
static double arrivalRate = 5;
static double averageDuration = 1;
static int maxTrials = 100000;
static int start = 500;
static double unicastPercent = 0;
static double anycastPercent = 0;
static double multicastPercent = 0;
static double multiChance = .3;
static double broadcastPercent = 1;
static int clientNodes = 26;

//Global Variables:
int globalTime = 0;
int droppedConnections = 0;
int arrivalCounter = 0;
double closestDeparture = 99999999999;
int eventlogIndex = 0;
int runs = 0;
int overlapCounter = 0;
struct connection {
	double arrivalTime;
	double departureTime;
	int startNode;
	int endNode;
	vector<int> endNodes;
	vector<int> usedLinks;
	string connectionType;
	int wavelengthNumber;
}newConnection;
struct link{
	vector<bool> isFree;
}newLink;
vector<int> startNodes;
vector<int> serverStartNodes;
vector<int> clientStartNodes;
vector<int> endNodes;
vector<int> serverEndNodes;
vector<int> clientEndNodes;
vector<int> overlapNodes;
vector<int> anycastNodes;
vector<double> interArrivalTimes;
vector<double> interDepartureTimes;
vector<link> systemLinks;
vector<connection> eventlog;
vector<connection> finalVector;
vector<connection> incomingConnections;
vector<ListDigraph::Arc> shortestPathVector;
vector<ListDigraph::Node> nodeVector;
vector<ListDigraph::Arc> pathVector;
ListDigraph g;
ListDigraph::NodeMap<string> nodeName(g);
ListDigraph::ArcMap<int> cost(g);
ListDigraph::ArcMap<int> name(g);
Dijkstra<ListDigraph> dijkstra1(g, cost);

//Function Headers:
void initialize();
void establishGraph();
void generateExponentialDistributions();
void generateIncomingConnections();
void generateLinks();
void add();
void remove();
void displayOutput();

//Functions:
int main(){
	srand(time(NULL));
	dijkstra1.init();
	establishGraph();
	initialize();
	while (arrivalCounter < maxTrials || !eventlog.empty()){
		for (int x = 0; x < eventlog.size(); x++){
			if (eventlog.at(x).departureTime < closestDeparture){
				closestDeparture = eventlog.at(x).departureTime;
				eventlogIndex = x;
			}
		}
		if (arrivalCounter < maxTrials){
			if (incomingConnections.at(arrivalCounter).arrivalTime < closestDeparture){
				add();
				arrivalCounter++;
				closestDeparture = 99999999999;
				pathVector.clear();
			}
			else{
				remove();
				closestDeparture = 99999999999;
			}
		}
		else{
			remove();
			closestDeparture = 99999999999;
		}
	}
	displayOutput();
	return 0;
}
void initialize(){
	globalTime = 0;
	droppedConnections = 0;
	arrivalCounter = 0;
	closestDeparture = 99999999999;
	eventlogIndex = 0;
	overlapCounter = 0;
	interArrivalTimes.clear();
	interDepartureTimes.clear();
	startNodes.clear();
	endNodes.clear();
	overlapNodes.clear();
	systemLinks.clear();
	eventlog.clear();
	finalVector.clear();
	incomingConnections.clear();
	generateExponentialDistributions();
	generateIncomingConnections();
	generateLinks();
}
void establishGraph(){
	// NSFNET TOPOLOGY
	/*	ListDigraph::Node Princeton = g.addNode(); nodeName[Princeton] = "Princeton"; nodeVector.push_back(Princeton);
	ListDigraph::Node GeorgiaTech = g.addNode(); nodeName[GeorgiaTech] = "GeorgiaTech"; nodeVector.push_back(GeorgiaTech);
	ListDigraph::Node Pittsburgh = g.addNode(); nodeName[Pittsburgh] = "Pittsburgh"; nodeVector.push_back(Pittsburgh);
	ListDigraph::Node Ithica = g.addNode(); nodeName[Ithica] = "Ithica"; nodeVector.push_back(Ithica);
	ListDigraph::Node Seattle = g.addNode(); nodeName[Seattle] = "Seattle"; nodeVector.push_back(Seattle);
	ListDigraph::Node PaloAlto = g.addNode(); nodeName[PaloAlto] = "PaloAlto"; nodeVector.push_back(PaloAlto);
	ListDigraph::Node SanDiego = g.addNode(); nodeName[SanDiego] = "SanDiego"; nodeVector.push_back(SanDiego);
	ListDigraph::Node SaltLake = g.addNode(); nodeName[SaltLake] = "SaltLake"; nodeVector.push_back(SaltLake);
	ListDigraph::Node Boulder = g.addNode(); nodeName[Boulder] = "Boulder"; nodeVector.push_back(Boulder);
	ListDigraph::Node Lincoln = g.addNode(); nodeName[Lincoln] = "Lincoln"; nodeVector.push_back(Lincoln);
	ListDigraph::Node Illinois = g.addNode(); nodeName[Illinois] = "Illinois"; nodeVector.push_back(Illinois);
	ListDigraph::Node AnnArbor = g.addNode(); nodeName[AnnArbor] = "AnnArbor"; nodeVector.push_back(AnnArbor);
	ListDigraph::Node Houston = g.addNode(); nodeName[Houston] = "Houston"; nodeVector.push_back(Houston);
	ListDigraph::Arc arc0 = g.addArc(AnnArbor, PaloAlto); ListDigraph::Arc arc15 = g.addArc(PaloAlto, AnnArbor);
	ListDigraph::Arc arc1 = g.addArc(Seattle, PaloAlto); ListDigraph::Arc arc16 = g.addArc(PaloAlto, Seattle);
	ListDigraph::Arc arc2 = g.addArc(Seattle, Boulder); ListDigraph::Arc arc17 = g.addArc(Boulder, Seattle);
	ListDigraph::Arc arc3 = g.addArc(Boulder, SaltLake); ListDigraph::Arc arc18 = g.addArc(SaltLake, Boulder);
	ListDigraph::Arc arc4 = g.addArc(Boulder, Illinois); ListDigraph::Arc arc19 = g.addArc(Illinois, Boulder);
	ListDigraph::Arc arc5 = g.addArc(Illinois, Lincoln); ListDigraph::Arc arc20 = g.addArc(Lincoln, Illinois);
	ListDigraph::Arc arc6 = g.addArc(Illinois, AnnArbor); ListDigraph::Arc arc21 = g.addArc(AnnArbor, Illinois);
	ListDigraph::Arc arc7 = g.addArc(Illinois, Houston); ListDigraph::Arc arc22 = g.addArc(Houston, Illinois);
	ListDigraph::Arc arc8 = g.addArc(AnnArbor, Ithica); ListDigraph::Arc arc23 = g.addArc(Ithica, AnnArbor);
	ListDigraph::Arc arc9 = g.addArc(AnnArbor, Pittsburgh); ListDigraph::Arc arc24 = g.addArc(Pittsburgh, AnnArbor);
	ListDigraph::Arc arc10 = g.addArc(Ithica, Princeton); ListDigraph::Arc arc25 = g.addArc(Princeton, Ithica);
	ListDigraph::Arc arc11 = g.addArc(Princeton, GeorgiaTech); ListDigraph::Arc arc26 = g.addArc(GeorgiaTech, Princeton);
	ListDigraph::Arc arc12 = g.addArc(GeorgiaTech, Houston); ListDigraph::Arc arc27 = g.addArc(Houston, GeorgiaTech);
	ListDigraph::Arc arc13 = g.addArc(Houston, SanDiego); ListDigraph::Arc arc28 = g.addArc(SanDiego, Houston);
	ListDigraph::Arc arc14 = g.addArc(SanDiego, PaloAlto); ListDigraph::Arc arc29 = g.addArc(PaloAlto, SanDiego);
	cost[arc0] = cost[arc15] = 5; name[arc0] = name[arc15] = 0;
	cost[arc1] = cost[arc16] = 5; name[arc1] = name[arc16] = 1;
	cost[arc2] = cost[arc17] = 5; name[arc2] = name[arc17] = 2;
	cost[arc3] = cost[arc18] = 5; name[arc3] = name[arc18] = 3;
	cost[arc4] = cost[arc19] = 5; name[arc4] = name[arc19] = 4;
	cost[arc5] = cost[arc20] = 5; name[arc5] = name[arc20] = 5;
	cost[arc6] = cost[arc21] = 5; name[arc6] = name[arc21] = 6;
	cost[arc7] = cost[arc22] = 5; name[arc7] = name[arc22] = 7;
	cost[arc8] = cost[arc23] = 5; name[arc8] = name[arc23] = 8;
	cost[arc9] = cost[arc24] = 5; name[arc9] = name[arc24] = 9;
	cost[arc10] = cost[arc25] = 5; name[arc10] = name[arc25] = 10;
	cost[arc11] = cost[arc26] = 5; name[arc11] = name[arc26] = 11;
	cost[arc12] = cost[arc27] = 5; name[arc12] = name[arc27] = 12;
	cost[arc13] = cost[arc28] = 5; name[arc13] = name[arc28] = 13;
	cost[arc14] = cost[arc29] = 5; name[arc14] = name[arc29] = 14;*/
	//	  ARPANET TOPOLOGY
	ListDigraph::Node ARPA = g.addNode(); nodeName[ARPA] = "ARPA"; nodeVector.push_back(ARPA);	
	ListDigraph::Node Stanford = g.addNode(); nodeName[Stanford] = "Stanford"; nodeVector.push_back(Stanford);
	ListDigraph::Node McClellan = g.addNode(); nodeName[McClellan] = "McClellan"; nodeVector.push_back(McClellan);  //0
	ListDigraph::Node Sri = g.addNode(); nodeName[Sri] = "Sri"; nodeVector.push_back(Sri);
	ListDigraph::Node Ames = g.addNode(); nodeName[Ames] = "Ames"; nodeVector.push_back(Ames);
	ListDigraph::Node Ames2 = g.addNode(); nodeName[Ames2] = "Ames2"; nodeVector.push_back(Ames2);
	ListDigraph::Node UCSB = g.addNode(); nodeName[UCSB] = "UCSB"; nodeVector.push_back(UCSB);						//5
	ListDigraph::Node UCLA = g.addNode(); nodeName[UCLA] = "UCLA"; nodeVector.push_back(UCLA);
	ListDigraph::Node SDC = g.addNode(); nodeName[SDC] = "SDC"; nodeVector.push_back(SDC);
	ListDigraph::Node USC = g.addNode(); nodeName[USC] = "USC"; nodeVector.push_back(USC);
	ListDigraph::Node RAND = g.addNode(); nodeName[RAND] = "RAND"; nodeVector.push_back(RAND);
	ListDigraph::Node Utah = g.addNode(); nodeName[Utah] = "Utah"; nodeVector.push_back(Utah);						//10
	ListDigraph::Node NOAA = g.addNode(); nodeName[NOAA] = "NOAA"; nodeVector.push_back(NOAA);
	ListDigraph::Node Tinker = g.addNode(); nodeName[Tinker] = "Tinker"; nodeVector.push_back(Tinker);
	ListDigraph::Node AFGWC = g.addNode(); nodeName[AFGWC] = "AFGWC"; nodeVector.push_back(AFGWC);
	ListDigraph::Node Illinois = g.addNode(); nodeName[Illinois] = "Illinois"; nodeVector.push_back(Illinois);
	ListDigraph::Node Case = g.addNode(); nodeName[Case] = "Case"; nodeVector.push_back(Case);						//15
	ListDigraph::Node Carnegie = g.addNode(); nodeName[Carnegie] = "Carnegie"; nodeVector.push_back(Carnegie);
	ListDigraph::Node RADC = g.addNode(); nodeName[RADC] = "RADC"; nodeVector.push_back(RADC);
	ListDigraph::Node Lincoln = g.addNode(); nodeName[Lincoln] = "Lincoln"; nodeVector.push_back(Lincoln);
	ListDigraph::Node MIT = g.addNode(); nodeName[MIT] = "MIT"; nodeVector.push_back(MIT);
	ListDigraph::Node BBN = g.addNode(); nodeName[BBN] = "BBN"; nodeVector.push_back(BBN);							//20
	ListDigraph::Node BBN2 = g.addNode(); nodeName[BBN2] = "BBN2"; nodeVector.push_back(BBN2);
	ListDigraph::Node Harvard = g.addNode(); nodeName[Harvard] = "Harvard"; nodeVector.push_back(Harvard);
	ListDigraph::Node Belvoir = g.addNode(); nodeName[Belvoir] = "Belvoir"; nodeVector.push_back(Belvoir);
	ListDigraph::Node SAAC = g.addNode(); nodeName[SAAC] = "SAAC"; nodeVector.push_back(SAAC);
	ListDigraph::Node Mitre = g.addNode(); nodeName[Mitre] = "Mitre"; nodeVector.push_back(Mitre);					//25
	ListDigraph::Node ETAC = g.addNode(); nodeName[ETAC] = "ETAC"; nodeVector.push_back(ETAC);
	ListDigraph::Node NBS = g.addNode(); nodeName[NBS] = "NBS"; nodeVector.push_back(NBS);							//28
	ListDigraph::Arc arc0 = g.addArc(Mitre, ARPA); ListDigraph::Arc arc32 = g.addArc(ARPA, Mitre);
	ListDigraph::Arc arc1 = g.addArc(ARPA, ETAC); ListDigraph::Arc arc33 = g.addArc(ETAC, ARPA);
	ListDigraph::Arc arc2 = g.addArc(McClellan, Utah); ListDigraph::Arc arc34 = g.addArc(Utah, McClellan);
	ListDigraph::Arc arc3 = g.addArc(McClellan, Sri); ListDigraph::Arc arc35 = g.addArc(Sri, McClellan);
	ListDigraph::Arc arc4 = g.addArc(Sri, Ames); ListDigraph::Arc arc36 = g.addArc(Ames, Sri);
	ListDigraph::Arc arc5 = g.addArc(Sri, UCSB); ListDigraph::Arc arc37 = g.addArc(UCSB, Sri);
	ListDigraph::Arc arc6 = g.addArc(Ames, Ames2); ListDigraph::Arc arc38 = g.addArc(Ames2, Ames);
	ListDigraph::Arc arc7 = g.addArc(Ames2, Stanford); ListDigraph::Arc arc39 = g.addArc(Stanford, Ames2);
	ListDigraph::Arc arc8 = g.addArc(Stanford, RAND); ListDigraph::Arc arc40 = g.addArc(RAND, Stanford);
	ListDigraph::Arc arc9 = g.addArc(UCSB, UCLA); ListDigraph::Arc arc41 = g.addArc(UCLA, UCSB);
	ListDigraph::Arc arc10 = g.addArc(UCLA, SDC); ListDigraph::Arc arc42 = g.addArc(SDC, UCLA);
	ListDigraph::Arc arc11 = g.addArc(UCLA, RAND); ListDigraph::Arc arc43 = g.addArc(RAND, UCLA);
	ListDigraph::Arc arc12 = g.addArc(SDC, USC); ListDigraph::Arc arc44 = g.addArc(USC, SDC);
	ListDigraph::Arc arc13 = g.addArc(RAND, Tinker); ListDigraph::Arc arc45 = g.addArc(Tinker, RAND);
	ListDigraph::Arc arc14 = g.addArc(USC, NOAA); ListDigraph::Arc arc46 = g.addArc(NOAA, USC);
	ListDigraph::Arc arc15 = g.addArc(Tinker, ETAC); ListDigraph::Arc arc47 = g.addArc(ETAC, Tinker);
	ListDigraph::Arc arc16 = g.addArc(NOAA, AFGWC); ListDigraph::Arc arc48 = g.addArc(AFGWC, NOAA);
	ListDigraph::Arc arc17 = g.addArc(Utah, Illinois); ListDigraph::Arc arc49 = g.addArc(Illinois, Utah);
	ListDigraph::Arc arc18 = g.addArc(AFGWC, Case); ListDigraph::Arc arc50 = g.addArc(Case, AFGWC);
	ListDigraph::Arc arc19 = g.addArc(Illinois, MIT); ListDigraph::Arc arc51 = g.addArc(MIT, Illinois);
	ListDigraph::Arc arc20 = g.addArc(Case, RADC); ListDigraph::Arc arc52 = g.addArc(RADC, Case);
	ListDigraph::Arc arc21 = g.addArc(Case, Carnegie); ListDigraph::Arc arc53 = g.addArc(Carnegie, Case);
	ListDigraph::Arc arc22 = g.addArc(RADC, Lincoln); ListDigraph::Arc arc54 = g.addArc(Lincoln, RADC);
	ListDigraph::Arc arc23 = g.addArc(Lincoln, MIT); ListDigraph::Arc arc55 = g.addArc(MIT, Lincoln);
	ListDigraph::Arc arc24 = g.addArc(MIT, BBN); ListDigraph::Arc arc56 = g.addArc(BBN, MIT);
	ListDigraph::Arc arc25 = g.addArc(BBN, BBN2); ListDigraph::Arc arc57 = g.addArc(BBN2, BBN);
	ListDigraph::Arc arc26 = g.addArc(BBN2, Harvard); ListDigraph::Arc arc58 = g.addArc(Harvard, BBN2);
	ListDigraph::Arc arc27 = g.addArc(Harvard, NBS); ListDigraph::Arc arc59 = g.addArc(NBS, Harvard);
	ListDigraph::Arc arc28 = g.addArc(NBS, ETAC); ListDigraph::Arc arc60 = g.addArc(ETAC, NBS);
	ListDigraph::Arc arc29 = g.addArc(Carnegie, Belvoir); ListDigraph::Arc arc61 = g.addArc(Belvoir, Carnegie);
	ListDigraph::Arc arc30 = g.addArc(Belvoir, SAAC); ListDigraph::Arc arc62 = g.addArc(SAAC, Belvoir);
	ListDigraph::Arc arc31 = g.addArc(SAAC, Mitre); ListDigraph::Arc arc63 = g.addArc(Mitre, SAAC);
	cost[arc0] = cost[arc32] = 5; name[arc0] = name[arc32] = 0;
	cost[arc1] = cost[arc33] = 5; name[arc1] = name[arc33] = 1;
	cost[arc2] = cost[arc34] = 5; name[arc2] = name[arc34] = 2;
	cost[arc3] = cost[arc35] = 5; name[arc3] = name[arc35] = 3;
	cost[arc4] = cost[arc36] = 5; name[arc4] = name[arc36] = 4;
	cost[arc5] = cost[arc37] = 5; name[arc5] = name[arc37] = 5;
	cost[arc6] = cost[arc38] = 5; name[arc6] = name[arc38] = 6;
	cost[arc7] = cost[arc39] = 5; name[arc7] = name[arc39] = 7;
	cost[arc8] = cost[arc40] = 5; name[arc8] = name[arc40] = 8;
	cost[arc9] = cost[arc41] = 5; name[arc9] = name[arc41] = 9;
	cost[arc10] = cost[arc42] = 5; name[arc10] = name[arc42] = 10;
	cost[arc11] = cost[arc43] = 5; name[arc11] = name[arc43] = 11;
	cost[arc12] = cost[arc44] = 5; name[arc12] = name[arc44] = 12;
	cost[arc13] = cost[arc45] = 5; name[arc13] = name[arc45] = 13;
	cost[arc14] = cost[arc46] = 5; name[arc14] = name[arc46] = 14;
	cost[arc15] = cost[arc47] = 5; name[arc15] = name[arc47] = 15;
	cost[arc16] = cost[arc48] = 5; name[arc16] = name[arc48] = 16;
	cost[arc17] = cost[arc49] = 5; name[arc17] = name[arc49] = 17;
	cost[arc18] = cost[arc50] = 5; name[arc18] = name[arc50] = 18;
	cost[arc19] = cost[arc51] = 5; name[arc19] = name[arc51] = 19;
	cost[arc20] = cost[arc52] = 5; name[arc20] = name[arc52] = 20;
	cost[arc21] = cost[arc53] = 5; name[arc21] = name[arc53] = 21;
	cost[arc22] = cost[arc54] = 5; name[arc22] = name[arc54] = 22;
	cost[arc23] = cost[arc55] = 5; name[arc23] = name[arc55] = 23;
	cost[arc24] = cost[arc56] = 5; name[arc24] = name[arc56] = 24;
	cost[arc25] = cost[arc57] = 5; name[arc25] = name[arc57] = 25;
	cost[arc26] = cost[arc58] = 5; name[arc26] = name[arc58] = 26;
	cost[arc27] = cost[arc59] = 5; name[arc27] = name[arc59] = 27;
	cost[arc28] = cost[arc60] = 5; name[arc28] = name[arc60] = 28;
	cost[arc29] = cost[arc61] = 5; name[arc29] = name[arc61] = 29;
	cost[arc30] = cost[arc62] = 5; name[arc30] = name[arc62] = 30;
	cost[arc31] = cost[arc63] = 5; name[arc31] = name[arc63] = 31;

}
void generateExponentialDistributions(){
	default_random_engine generator;
	generator.seed(time(NULL));
	exponential_distribution<double> arrivalDistribution(arrivalRate);
	for (int i = 0; i < maxTrials; i++){
		interArrivalTimes.push_back(arrivalDistribution(generator));
	}
	exponential_distribution<double> departureDistribution(1 / averageDuration);
	for (int i = 0; i < maxTrials; i++){
		interDepartureTimes.push_back(departureDistribution(generator));
	}
	uniform_int_distribution<int> arrivalNodeDistribution(0, numberNodes-1);
	for (int i = 0; i < maxTrials; i++){
		startNodes.push_back(arrivalNodeDistribution(generator));
	}
	uniform_int_distribution<int> serverStartNodeDistribution(0, 1);
	for (int i = 0; i < maxTrials; i++){
		serverStartNodes.push_back(serverStartNodeDistribution(generator));
	}
	uniform_int_distribution<int> clientStartNodeDistribution(2, numberNodes-1);
	for (int i = 0; i < maxTrials; i++){
		clientStartNodes.push_back(clientStartNodeDistribution(generator));
	}
	uniform_int_distribution<int> departureNodeDistribution(0, numberNodes-1);
	for (int i = 0; i < maxTrials; i++){
		endNodes.push_back(departureNodeDistribution(generator));
	}
	uniform_int_distribution<int> serverEndNodeDistribution(0, 1);
	for (int i = 0; i < maxTrials; i++){
		serverEndNodes.push_back(serverEndNodeDistribution(generator));
	}
	uniform_int_distribution<int> clientEndNodeDistribution(2, numberNodes - 1);
	for (int i = 0; i < maxTrials; i++){
		clientEndNodes.push_back(clientEndNodeDistribution(generator));
	}
	uniform_int_distribution<int> overlapNodeDistribution(0, numberNodes-1);
	for (int i = 0; i < maxTrials; i++){
		overlapNodes.push_back(overlapNodeDistribution(generator));
	}
}
void generateIncomingConnections(){
	double loopTime = 0;
	for (int i = 0; i < maxTrials; i++){
		double connectionRand = ((double)(rand() %100))/100;
		incomingConnections.push_back(newConnection);
		incomingConnections.at(i).arrivalTime = (loopTime + interArrivalTimes.at(i));
		incomingConnections.at(i).departureTime = loopTime + interArrivalTimes.at(i) + interDepartureTimes.at(i);
		loopTime += interArrivalTimes.at(i);
		if (connectionRand < unicastPercent){
			if (state == 1 || state == 3){
				incomingConnections.at(i).startNode = startNodes.at(i);
				do{
					incomingConnections.at(i).endNode = endNodes.at(i);
					if (incomingConnections.at(i).startNode == incomingConnections.at(i).endNode){
						incomingConnections.at(i).endNode = overlapNodes.at(overlapCounter);
						overlapCounter++;
					}
				} while (incomingConnections.at(i).startNode == incomingConnections.at(i).endNode);
			}
			incomingConnections.at(i).connectionType = "Unicast";
		}
		else if (connectionRand < (unicastPercent + anycastPercent)){
				incomingConnections.at(i).startNode = clientStartNodes.at(i);
				dijkstra1.run(nodeVector.at(incomingConnections.at(i).startNode), nodeVector.at(0));
				int tempDistance0 = 0;
				ListDigraph::Node tempNode = nodeVector.at(0);
				do{
					pathVector.push_back(dijkstra1.predArc(tempNode));
					tempDistance0 += cost[dijkstra1.predArc(tempNode)];
					tempNode = dijkstra1.predNode(tempNode);
				} while (tempDistance0 != dijkstra1.dist(nodeVector.at(0)));
				dijkstra1.run(nodeVector.at(incomingConnections.at(i).startNode), nodeVector.at(1));
				int tempDistance1 = 0;
				tempNode = nodeVector.at(1);
				do{
					pathVector.push_back(dijkstra1.predArc(tempNode));
					tempDistance1 += cost[dijkstra1.predArc(tempNode)];
					tempNode = dijkstra1.predNode(tempNode);
				} while (tempDistance1 != dijkstra1.dist(nodeVector.at(1)));
				if (tempDistance0 > tempDistance1){
					incomingConnections.at(i).endNode = 1;
				}
				if (tempDistance0 < tempDistance1){
					incomingConnections.at(i).endNode = 0;
				}
				if (tempDistance0 = tempDistance1){
					incomingConnections.at(i).endNode = rand()%2;
				}
			incomingConnections.at(i).connectionType = "Anycast";
		}
		else if (connectionRand < (unicastPercent + anycastPercent + multicastPercent)){
			incomingConnections.at(i).startNode = ((rand() % 2));
			for (int k = 2; k < (2+clientNodes); k++){
				double tempMulti = (double)(rand() % (100)) / 100;
				if (multiChance > tempMulti){
					incomingConnections.at(i).endNodes.push_back(k);
				}
			}
			incomingConnections.at(i).connectionType = "Multicast";
		}
		else if (connectionRand < (unicastPercent + anycastPercent + broadcastPercent)){
			incomingConnections.at(i).startNode = ((rand() % 2));
			incomingConnections.at(i).connectionType = "Broadcast";
		}
		incomingConnections.at(i).wavelengthNumber = -1;
	}
}
void generateLinks(){
	for (int i = 0; i < numberSystemLinks; i++){
		for (int j = 0; j < numberWavelengths; j++){
			newLink.isFree.push_back(true);
		}
		systemLinks.push_back(newLink);
		newLink.isFree.clear();
	}
}
void remove(){
	globalTime = eventlog.at(eventlogIndex).departureTime;
	finalVector.push_back(eventlog.at(eventlogIndex));
	if (state == 1 || state == 2){
		for (int k = 0; k < eventlog.at(eventlogIndex).usedLinks.size(); k++){
			systemLinks.at(eventlog.at(eventlogIndex).usedLinks.at(k)).isFree.at(eventlog.at(eventlogIndex).wavelengthNumber) = true;
		}
	}
	eventlog.erase(eventlog.begin() + eventlogIndex);
}
void add(){
	if (incomingConnections.at(arrivalCounter).connectionType == "Unicast" || incomingConnections.at(arrivalCounter).connectionType == "Anycast"){
		dijkstra1.run(nodeVector.at(incomingConnections.at(arrivalCounter).startNode), nodeVector.at(incomingConnections.at(arrivalCounter).endNode));
		int tempDistance = 0;
		ListDigraph::Node tempNode = nodeVector.at(incomingConnections.at(arrivalCounter).endNode);
		do{
			pathVector.push_back(dijkstra1.predArc(tempNode));
			tempDistance += cost[dijkstra1.predArc(tempNode)];
			tempNode = dijkstra1.predNode(tempNode);
		} while (tempDistance != dijkstra1.dist(nodeVector.at(incomingConnections.at(arrivalCounter).endNode)));
		globalTime = incomingConnections.at(arrivalCounter).arrivalTime;
		if (state == 1){
			for (int i = 0; i < numberWavelengths; i++){
				int freeLinks = 0;
				for (int j = 0; j < pathVector.size(); j++){
					if (systemLinks.at(name[pathVector.at(j)]).isFree.at(i) == false){
						break;
					}
					else{
						freeLinks++;
					}
				}
				if (freeLinks == pathVector.size()){
					incomingConnections.at(arrivalCounter).wavelengthNumber = i;
					for (int j = 0; j < pathVector.size(); j++){
						systemLinks.at(name[pathVector.at(j)]).isFree.at(i) = false;
						incomingConnections.at(arrivalCounter).usedLinks.push_back(name[pathVector.at(j)]);
					}
					eventlog.push_back(incomingConnections.at(arrivalCounter));
					break;
				}
			}
			if (incomingConnections.at(arrivalCounter).wavelengthNumber == -1){
				if (arrivalCounter >= start){
					droppedConnections++;
				}
			}
		}
	}
	if (incomingConnections.at(arrivalCounter).connectionType == "Multicast"){
		for (int k = 0; k < incomingConnections.at(arrivalCounter).endNodes.size(); k++){
			dijkstra1.run(nodeVector.at(incomingConnections.at(arrivalCounter).startNode), nodeVector.at(incomingConnections.at(arrivalCounter).endNodes.at(k)));
			int tempDistance = 0;
			ListDigraph::Node tempNode = nodeVector.at(incomingConnections.at(arrivalCounter).endNodes.at(k));
			do{
				bool vectorCheck = true;
				for (int x = 0; x < pathVector.size(); x++){
					if (pathVector.at(x) == dijkstra1.predArc(tempNode)){
						vectorCheck = false;
					}
				}
				if (vectorCheck = true){
					pathVector.push_back(dijkstra1.predArc(tempNode));
				}
				tempDistance += cost[dijkstra1.predArc(tempNode)];
				tempNode = dijkstra1.predNode(tempNode);
			} while (tempDistance != dijkstra1.dist(nodeVector.at(incomingConnections.at(arrivalCounter).endNodes.at(k))));
		}
		globalTime = incomingConnections.at(arrivalCounter).arrivalTime;
		if (state == 1){
			for (int i = 0; i < numberWavelengths; i++){
				int freeLinks = 0;
				for (int j = 0; j < pathVector.size(); j++){
					if (systemLinks.at(name[pathVector.at(j)]).isFree.at(i) == false){
						break;
					}
					else{
						freeLinks++;
					}
				}
				if (freeLinks == pathVector.size()){
					incomingConnections.at(arrivalCounter).wavelengthNumber = i;
					for (int j = 0; j < pathVector.size(); j++){
						systemLinks.at(name[pathVector.at(j)]).isFree.at(i) = false;
						incomingConnections.at(arrivalCounter).usedLinks.push_back(name[pathVector.at(j)]);
					}
					eventlog.push_back(incomingConnections.at(arrivalCounter));
					break;
				}
			}
			if (incomingConnections.at(arrivalCounter).wavelengthNumber == -1){
				if (arrivalCounter >= start){
					droppedConnections++;
				}
			}
		}
	}
	if (incomingConnections.at(arrivalCounter).connectionType == "Broadcast"){
		globalTime = incomingConnections.at(arrivalCounter).arrivalTime;
		for (int i = 0; i < numberWavelengths; i++){
			int freeLinks = 0;
			for (int j = 0; j < 32; j++){
				if (systemLinks.at(j).isFree.at(i) == false){
					break;
				}
				else{
					freeLinks++;
				}
			}
			if (freeLinks == 32){
				incomingConnections.at(arrivalCounter).wavelengthNumber = i;
				for (int j = 0; j < 32; j++){
					systemLinks.at(j).isFree.at(i) = false;
					incomingConnections.at(arrivalCounter).usedLinks.push_back(j);
				}
				eventlog.push_back(incomingConnections.at(arrivalCounter));
				break;
			}
		}
		if (incomingConnections.at(arrivalCounter).wavelengthNumber == -1){
			if (arrivalCounter >= start){
				droppedConnections++;
			}
		}
	}
}


void displayOutput(){
	//	cout << droppedConnections << endl;
	//	cout << (arrivalCounter-start) << endl;
	//	cout << "System Time" << globalTime << endl;
	//	cout << "Final Vector Size: " << finalVector.size() << endl;
	cout << "ECE 561 - Project 2 - Extra Credit - .Exe:" << endl;
	cout << "Example Executable - Part D" << endl;
	cout << "Modelled Network - ARPANET - 29 Node Map " << endl;
	cout << "Number of Wavelengths: " << numberWavelengths << endl;
	cout << "Number of Samples Run: " << maxTrials << endl;
	cout << "Number of Iterations Run Before Recording Data: " << start << endl;
	cout << "Connection Drop Rate: " << (double)droppedConnections / (double)(arrivalCounter - start) << endl;
		system("PAUSE");
}