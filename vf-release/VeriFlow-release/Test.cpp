/*
 * Test.cpp
 *
 *  Created on: Jul 15, 2012
 *      Author: khurshid
 *
 * This file is protected by the VeriFlow Research License Agreement
 * available at http://www.cs.illinois.edu/~khurshi1/projects/veriflow/veriflow-research-license-agreement.txt.
 * A copy of this agreement is also included in this package.
 *
 * Copyright (c) 2012-2013 by
 * The Board of Trustees of the University of Illinois.
 * All rights reserved.
 */

#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sys/time.h>
#include <dirent.h>
#include "Test.h"
#include "Trie.h"
#include "VeriFlow.h"
#include "Rule.h"
#include "EquivalenceClass.h"
#include "../veriflow-util/openflow.h"
#include "../veriflow-util/StringTokenizer.h"

using namespace std;

void Test::testVerification()
{
	double updateTime, packetClassSearchTime, graphBuildTime, queryTime, verificationTime;
	unsigned long ecCount;

	struct timeval start, end;
	double usecTime, seconds, useconds;

	Rule rule1, rule2, rule3, rule4, rule5, rule6, rule7;

	rule1.type = FORWARDING;
	rule1.fieldValue[DL_SRC] = "0:0:0:0:0:0";
	rule1.fieldMask[DL_SRC] = "0:0:0:0:0:0";
	rule1.fieldValue[DL_DST] = "0:0:0:0:0:0";
	rule1.fieldMask[DL_DST] = "0:0:0:0:0:0";
	rule1.fieldValue[NW_SRC] = "0.0.0.0";
	rule1.fieldMask[NW_SRC] = "0.0.0.0";
	rule1.fieldValue[NW_DST] = "20.1.0.0";
	rule1.fieldMask[NW_DST] = "255.255.0.0";

	// rule1.wildcards = 0;
	rule1.wildcards = (OFPFW_ALL & ~OFPFW_TP_SRC); // OFPFW_TP_SRC;
	rule1.fieldValue[TP_SRC] = "80";
	rule1.fieldMask[TP_SRC] = "65535";
	// rule1.fieldValue[TP_DST] = "8080";
	// rule1.fieldMask[TP_DST] = "65535";

	rule1.location = "102.0.0.1";
	rule1.nextHop = "103.0.0.1";
	rule1.priority = 3;

	rule2.type = FORWARDING;
	rule2.fieldValue[DL_SRC] = "0:0:0:0:0:0";
	rule2.fieldMask[DL_SRC] = "0:0:0:0:0:0";
	rule2.fieldValue[DL_DST] = "0:0:0:0:0:0";
	rule2.fieldMask[DL_DST] = "0:0:0:0:0:0";
	rule2.fieldValue[NW_SRC] = "0.0.0.0";
	rule2.fieldMask[NW_SRC] = "0.0.0.0";
	rule2.fieldValue[NW_DST] = "20.1.0.0";
	rule2.fieldMask[NW_DST] = "255.255.0.0";

	// rule2.wildcards = 0;
	rule2.wildcards = (OFPFW_ALL & ~OFPFW_TP_DST); // OFPFW_TP_DST;
	rule2.fieldValue[TP_DST] = "5555";
	rule2.fieldMask[TP_DST] = "65535";

	rule2.location = "102.0.0.1";
	rule2.nextHop = "103.0.0.1";
	rule2.priority = 2;

	rule3.type = FORWARDING;
	rule3.fieldValue[DL_SRC] = "0:0:0:0:0:0";
	rule3.fieldMask[DL_SRC] = "0:0:0:0:0:0";
	rule3.fieldValue[DL_DST] = "0:0:0:0:0:0";
	rule3.fieldMask[DL_DST] = "0:0:0:0:0:0";
	rule3.fieldValue[NW_SRC] = "0.0.0.0";
	rule3.fieldMask[NW_SRC] = "0.0.0.0";
	rule3.fieldValue[NW_DST] = "20.1.0.0";
	rule3.fieldMask[NW_DST] = "255.255.0.0";

	// rule3.wildcards = 0;
	rule3.wildcards = OFPFW_ALL; // OFPFW_TP_SRC;
	// rule3.fieldValue[TP_SRC] = "80";
	// rule3.fieldMask[TP_SRC] = "65535";
	// rule3.fieldValue[TP_DST] = "8080";
	// rule3.fieldMask[TP_DST] = "65535";

	rule3.location = "102.0.0.1";
	rule3.nextHop = "104.0.0.1";
	rule3.priority = 1;

	rule4.type = FORWARDING;
	rule4.fieldValue[DL_SRC] = "0:0:0:0:0:0";
	rule4.fieldMask[DL_SRC] = "0:0:0:0:0:0";
	rule4.fieldValue[DL_DST] = "0:0:0:0:0:0";
	rule4.fieldMask[DL_DST] = "0:0:0:0:0:0";
	rule4.fieldValue[NW_SRC] = "0.0.0.0";
	rule4.fieldMask[NW_SRC] = "0.0.0.0";
	rule4.fieldValue[NW_DST] = "20.1.0.0";
	rule4.fieldMask[NW_DST] = "255.255.0.0";

	// rule4.wildcards = 0;
	rule4.wildcards = OFPFW_ALL; // OFPFW_TP_SRC;
	// rule4.fieldValue[TP_SRC] = "80";
	// rule4.fieldMask[TP_SRC] = "65535";
	// rule4.fieldValue[TP_DST] = "8080";
	// rule4.fieldMask[TP_DST] = "65535";

	rule4.location = "103.0.0.1";
	rule4.nextHop = "103.0.0.1";
	rule4.priority = 1;

	rule5.type = FORWARDING;
	rule5.fieldValue[DL_SRC] = "0:0:0:0:0:0";
	rule5.fieldMask[DL_SRC] = "0:0:0:0:0:0";
	rule5.fieldValue[DL_DST] = "0:0:0:0:0:0";
	rule5.fieldMask[DL_DST] = "0:0:0:0:0:0";
	rule5.fieldValue[NW_SRC] = "0.0.0.0";
	rule5.fieldMask[NW_SRC] = "0.0.0.0";
	rule5.fieldValue[NW_DST] = "20.1.0.0";
	rule5.fieldMask[NW_DST] = "255.255.0.0";

	// rule5.wildcards = 0;
	rule5.wildcards = OFPFW_ALL; // OFPFW_TP_SRC;
	// rule5.fieldValue[TP_SRC] = "80";
	// rule5.fieldMask[TP_SRC] = "65535";
	// rule5.fieldValue[TP_DST] = "8080";
	// rule5.fieldMask[TP_DST] = "65535";

	rule5.location = "104.0.0.1";
	rule5.nextHop = "104.0.0.1";
	rule5.priority = 1;

	rule6.type = FORWARDING;
	rule6.fieldValue[DL_SRC] = "0:0:0:0:0:0";
	rule6.fieldMask[DL_SRC] = "0:0:0:0:0:0";
	rule6.fieldValue[DL_DST] = "0:0:0:0:0:0";
	rule6.fieldMask[DL_DST] = "0:0:0:0:0:0";
	rule6.fieldValue[NW_SRC] = "0.0.0.0";
	rule6.fieldMask[NW_SRC] = "0.0.0.0";
	rule6.fieldValue[NW_DST] = "20.1.0.0";
	rule6.fieldMask[NW_DST] = "255.255.0.0";

	// rule6.wildcards = 0;
	rule6.wildcards = (OFPFW_ALL & ~OFPFW_TP_SRC); // OFPFW_TP_SRC;
	rule6.fieldValue[TP_SRC] = "80";
	rule6.fieldMask[TP_SRC] = "65535";

	rule6.location = "104.0.0.1";
	rule6.nextHop = "104.0.0.1";
	rule6.priority = 2;

	rule7.type = FORWARDING;
	rule7.fieldValue[DL_SRC] = "0:0:0:0:0:0";
	rule7.fieldMask[DL_SRC] = "0:0:0:0:0:0";
	rule7.fieldValue[DL_DST] = "0:0:0:0:0:0";
	rule7.fieldMask[DL_DST] = "0:0:0:0:0:0";
	rule7.fieldValue[NW_SRC] = "0.0.0.0";
	rule7.fieldMask[NW_SRC] = "0.0.0.0";
	rule7.fieldValue[NW_DST] = "20.1.0.0";
	rule7.fieldMask[NW_DST] = "255.255.0.0";

	// rule7.wildcards = 0;
	rule7.wildcards = OFPFW_ALL; // OFPFW_TP_SRC;
	// rule7.fieldValue[TP_SRC] = "80";
	// rule7.fieldMask[TP_SRC] = "65535";
	// rule7.fieldValue[TP_DST] = "8080";
	// rule7.fieldMask[TP_DST] = "65535";

	rule7.location = "101.0.0.1";
	rule7.nextHop = "102.0.0.1";
	rule7.priority = 1;

	VeriFlow veriflow;
	veriflow.addRule(rule1);
	veriflow.addRule(rule2);
	veriflow.addRule(rule3);
	veriflow.addRule(rule4);
	veriflow.addRule(rule5);
	veriflow.addRule(rule6);
	// veriflow.removeRule(rule5);
	// veriflow.removeRule(rule1);

	gettimeofday(&start, NULL);
	veriflow.verifyRule(rule7, OFPFC_ADD, updateTime, packetClassSearchTime, graphBuildTime, queryTime, ecCount, stdout);
	gettimeofday(&end, NULL);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	usecTime = (seconds * 1000000) + useconds;
	verificationTime = usecTime;

	fprintf(stdout, "\nRuleSet 1 VerificationTime(usec) %f EquivalenceClassCount %lu UpdateTime(usec) %f PacketSearchTime(usec) %f GraphBuildTime(usec) %f QueryTime(usec) %f\n",
		verificationTime, ecCount, updateTime, packetClassSearchTime, graphBuildTime, queryTime);
}

void testRocketfuelRouteViewsDataset()
{
	fprintf(stdout, "Make sure to modify VeriFlow::updateForwardingGraphs() and Trie::getForwardingGraph() to handle the internal subnets (10.*.*.*).\n");
	fflush(stdout);

	string baseDir = "/home/mininet/ADNS1/data/AS-1755/";
	DIR *dp;
	struct dirent *dirp;
	vector< string > deviceFileList;

	if((dp = opendir(baseDir.c_str())) == NULL)
	{
		fprintf(stderr, "Error opening %s\n", baseDir.c_str());
		return;
	}

	while((dirp = readdir(dp)))
	{
		if((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0))
		{
			continue;
		}

		if(strchr(dirp->d_name, 'R') == dirp->d_name)
		{
			string fileName = baseDir + "/" + dirp->d_name;
			deviceFileList.push_back(fileName);
		}
	}

	closedir(dp);

	fprintf(stdout, "Total device count: %lu\n", deviceFileList.size());
	fflush(stdout);

	char buffer[1024];
	VeriFlow veriflow;

	struct timeval start, end;
	double usecTime, seconds, useconds;

	gettimeofday(&start, NULL);
	for(unsigned int i = 0; i < deviceFileList.size(); i++)
	{
		fprintf(stdout, "[%u] Processing %s\n", i, deviceFileList[i].c_str());
		fflush(stdout);
		ifstream fin(deviceFileList[i].c_str());
		while(fin.eof() == false)
		{
			fin.getline(buffer, 1023);
			if(strlen(buffer) == 0)
			{
				continue;
			}

			// Old format: 190.144.168.0 255.255.255.0 atla 64.57.28.7 1
			// New format: 0 I 10.0.5.108 255.255.255.252 10.0.0.1 10.0.0.37 125 1
			// New format: 0 B 62.76.36.0/23 255.255.254.0 10.0.5.114 192.203.116.253 280 1

			StringTokenizer st(buffer, " ");
			Rule rule;
			st.nextToken();
			st.nextToken();

			rule.type = FORWARDING;
			rule.fieldValue[DL_SRC] = "0:0:0:0:0:0";
			rule.fieldMask[DL_SRC] = "0:0:0:0:0:0";
			rule.fieldValue[DL_DST] = "0:0:0:0:0:0";
			rule.fieldMask[DL_DST] = "0:0:0:0:0:0";
			rule.fieldValue[NW_SRC] = "0.0.0.0";
			rule.fieldMask[NW_SRC] = "0.0.0.0";

			rule.fieldValue[NW_DST] = st.nextToken();
			if(rule.fieldValue[NW_DST].find('/') != string::npos)
			{
				rule.fieldValue[NW_DST] = rule.fieldValue[NW_DST].substr(0, rule.fieldValue[NW_DST].find('/'));
			}

			rule.fieldMask[NW_DST] = st.nextToken();
			rule.wildcards = OFPFW_ALL;
			rule.location = st.nextToken();
			rule.nextHop = st.nextToken();
			st.nextToken();
			rule.priority = atoi(st.nextToken().c_str());
			// rule.computeEquivalenceClass();

			veriflow.addRule(rule);
		}

		fin.close();
	}
	gettimeofday(&end, NULL);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	usecTime = (seconds * 1000000) + useconds;
	double treeBuildTime = usecTime;

	// Apply BGP route updates.
	string bgpUpdateFileName = baseDir + "/bgp-rules.txt";
	string resultFileName = baseDir + "/bgp-trace-result.txt";
	ifstream fin(bgpUpdateFileName.c_str());
	//ofstream fout(resultFileName.c_str());

	fprintf(stdout, "Start reading BGP update trace file.\n");
	fflush(stdout);

	vector< string > vUpdateTrace;
	while(fin.eof() == false)
	{
		fin.getline(buffer, 1023);
		if(strlen(buffer) == 0)
		{
			continue;
		}

		vUpdateTrace.push_back(buffer);
	}

	fin.close();

	fprintf(stdout, "Completed reading trace file. Update count: %lu.\n", vUpdateTrace.size());
	fflush(stdout);

	// 1322697618 A 31.134.208.0 255.255.248.0 10.0.5.22 10.0.1.114 315 1
	//      1     2       3            4           5         6       7  8
	string time = "";
	string type = "";
	Rule rule;
	int totalRuleCount = 0;

        FILE* fout = fopen("vftime", "w");
	for(unsigned int i = 0; i < vUpdateTrace.size(); i++)
	{
		string traceLine = vUpdateTrace[i];
		fprintf(stdout, "[%u] %s\n", (i + 1), traceLine.c_str());
		fflush(stdout);

		StringTokenizer st(traceLine, " ");
		time = st.nextToken();
		type = st.nextToken();

		if((type.compare("A") != 0) && (type.compare("W") != 0))
		{
			continue;
		}

		int command = OFPFC_ADD;
		if(type.compare("W") == 0)
		{
			command = OFPFC_DELETE_STRICT;
		}

		rule.type = FORWARDING;
		rule.fieldValue[DL_SRC] = "0:0:0:0:0:0";
		rule.fieldMask[DL_SRC] = "0:0:0:0:0:0";
		rule.fieldValue[DL_DST] = "0:0:0:0:0:0";
		rule.fieldMask[DL_DST] = "0:0:0:0:0:0";
		rule.fieldValue[NW_SRC] = "0.0.0.0";
		rule.fieldMask[NW_SRC] = "0.0.0.0";

		rule.fieldValue[NW_DST] = st.nextToken();
		if(rule.fieldValue[NW_DST].find('/') != string::npos)
		{
			rule.fieldValue[NW_DST] = rule.fieldValue[NW_DST].substr(0, rule.fieldValue[NW_DST].find('/'));
		}

		rule.fieldMask[NW_DST] = st.nextToken();
		rule.wildcards = OFPFW_ALL;
		rule.location = st.nextToken();
		rule.nextHop = st.nextToken();
		st.nextToken();
		rule.priority = atoi(st.nextToken().c_str());
		// rule.computeEquivalenceClass();
		//fprintf(stdout, "%d\n", ++totalRuleCount);

		double updateTime = 0, packetClassSearchTime = 0, graphBuildTime = 0, queryTime = 0;
		bool reachability = false;
		unsigned long ecCount = 0;

		gettimeofday(&start, NULL);
		veriflow.verifyRule(rule, command, updateTime, packetClassSearchTime, graphBuildTime, queryTime, ecCount, stderr);
		gettimeofday(&end, NULL);

		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;
		usecTime = (seconds * 1000000) + useconds;
		double verificationTime = usecTime;
                
                //fprintf(fout, "%f\n", command, usecTime);

		//fout << "UpdateId " << totalRuleCount
		//		<< " ArrivalTime(s) " << time
		//		<< " VerificationTime(usec) " << (long)verificationTime
		//		<< " EquivalenceClassCount " << ecCount
		//		<< " DstAddr " << rule.fieldValue[NW_DST]
		//		<< " DstAddrMask " << rule.fieldMask[NW_DST]
		//		<< " Type " << type
		//		<< " UpdateTime(usec) " << (long)updateTime
		//		<< " PacketSearchTime(usec) " << (long)packetClassSearchTime
		//		<< " GraphBuildTime(usec) " << (long)graphBuildTime
		//		<< " QueryTime(usec) " << (long)queryTime
		//		<< " Reachability " << reachability
		//		<< endl;

		if(command == OFPFC_ADD)
		{
			veriflow.addRule(rule);
                        fprintf(fout, "a %f\n", usecTime);
		}
		else if(command == OFPFC_DELETE_STRICT)
		{
			veriflow.removeRule(rule);
                        fprintf(fout, "d %f\n", usecTime);
		}
	}

	fprintf(stdout, "Tree build time (usec): %f\n", treeBuildTime);
	fprintf(stdout, "Total rule count: %d\n", totalRuleCount);

	fclose(fout);
	//fout.close();
}

static std::map<std::string, std::map<int, std::string> > port_map;


void BugCoverTest()
{
	VeriFlow veriflow;
  char* topo_file = "/home/wzhou10/projects/veriflow/code/VeriFlow-uncertain/test/fattree_10.txt";
  // extract the topology: (outport, ip) -> next hop;
  FILE* ftopo = fopen(topo_file, "r");

  char line[1024];
  int lineno = 1;
  while (fgets(line, 1024, ftopo) != NULL) {

    char *tmp;

    char* str_id = strtok_r(line, " ", &tmp);
    if (!str_id) {
      fprintf(stderr, "can not read id on line %d\n", lineno);
      exit(1);
    }

    char* str_ip = strtok_r(NULL, " ", &tmp);
    if (!str_ip) {
      fprintf(stderr, "can not read ip on line %d\n", lineno);
      exit(1);
    }

    string host_ip(str_ip);

    while ((str_id = strtok_r(NULL, " ", &tmp)) != NULL) {
      int host_port;
      if (sscanf(str_id, "%d", &host_port) != 1) {
        fprintf(stderr, "can not parse port %s\n", str_id);
        exit(1);
      }
      str_ip = strtok_r(NULL, " ", &tmp);
      if (!str_ip) {
        fprintf(stderr, "can not parse port %s\n", str_ip);
        exit(1);
      }
      string next_hop(str_ip);
      port_map[host_ip][host_port] = next_hop;
    }
    lineno++;
  }


  char* trace_file = "/home/wzhou10/projects/veriflow/code/VeriFlow-uncertain-release/bug_rules.txt";
  ifstream fin(trace_file);

	string time = "";
	string type = "";
  char buffer[1024];
  vector< string > vUpdateTrace;
  while(fin.eof() == false)
  {
    fin.getline(buffer, 1023);
    if(strlen(buffer) == 0)
    {
      continue;
    }

    vUpdateTrace.push_back(buffer);
  }

  fin.close();

  // 1322697618 A 31.134.208.0 255.255.248.0 10.0.5.22 10.0.1.114 315 1
  //      1     2       3            4           5         6       7  8
  Rule rule;

  for(unsigned int i = 0; i < vUpdateTrace.size(); i++)
  {
    string traceLine = vUpdateTrace[i];
    fprintf(stdout, "[%u] %s\n", (i + 1), traceLine.c_str());
    fflush(stdout);

    StringTokenizer st(traceLine, " ");
    time = st.nextToken();
    type = st.nextToken();

    if((type.compare("A") != 0) && (type.compare("W") != 0))
    {
      continue;
    }

    int command = OFPFC_ADD;
    if(type.compare("W") == 0)
    {
      command = OFPFC_DELETE_STRICT;
    }

    rule.type = FORWARDING;
    rule.fieldValue[DL_SRC] = "0:0:0:0:0:0";
    rule.fieldMask[DL_SRC] = "0:0:0:0:0:0";
    rule.fieldValue[DL_DST] = "0:0:0:0:0:0";
    rule.fieldMask[DL_DST] = "0:0:0:0:0:0";
    rule.fieldValue[NW_SRC] = "0.0.0.0";
    rule.fieldMask[NW_SRC] = "0.0.0.0";

    rule.fieldValue[NW_DST] = st.nextToken();
    if(rule.fieldValue[NW_DST].find('/') != string::npos)
    {
      rule.fieldValue[NW_DST] = rule.fieldValue[NW_DST].substr(0, rule.fieldValue[NW_DST].find('/'));
    }

    rule.fieldMask[NW_DST] = st.nextToken();
    rule.wildcards = OFPFW_ALL;
    rule.location = st.nextToken();
    string str_outport = st.nextToken();
    int port_id;
    sscanf(str_outport.c_str(), "%d", &port_id);
    rule.nextHop = port_map[rule.fieldValue[NW_DST]][port_id];
    st.nextToken();
    rule.priority = atoi(st.nextToken().c_str());
    // rule.computeEquivalenceClass();
    //fprintf(stdout, "%d\n", ++totalRuleCount);

    double updateTime = 0, packetClassSearchTime = 0, graphBuildTime = 0, queryTime = 0;
    bool reachability = false;
    unsigned long ecCount = 0;

    struct timeval start, end;
    gettimeofday(&start, NULL);
    veriflow.verifyRule(rule, command, updateTime, packetClassSearchTime, graphBuildTime, queryTime, ecCount, stderr);
    gettimeofday(&end, NULL);

    uint64_t seconds  = end.tv_sec  - start.tv_sec;
    uint64_t useconds = end.tv_usec - start.tv_usec;
    uint64_t usecTime = (seconds * 1000000) + useconds;
    double verificationTime = usecTime;

    //fprintf(fout, "%f\n", command, usecTime);

    //fout << "UpdateId " << totalRuleCount
    //		<< " ArrivalTime(s) " << time
    //		<< " VerificationTime(usec) " << (long)verificationTime
    //		<< " EquivalenceClassCount " << ecCount
    //		<< " DstAddr " << rule.fieldValue[NW_DST]
    //		<< " DstAddrMask " << rule.fieldMask[NW_DST]
    //		<< " Type " << type
    //		<< " UpdateTime(usec) " << (long)updateTime
    //		<< " PacketSearchTime(usec) " << (long)packetClassSearchTime
    //		<< " GraphBuildTime(usec) " << (long)graphBuildTime
    //		<< " QueryTime(usec) " << (long)queryTime
    //		<< " Reachability " << reachability
    //		<< endl;

    if(command == OFPFC_ADD)
    {
      veriflow.addRule(rule);
    }
    else if(command == OFPFC_DELETE_STRICT)
    {
      veriflow.removeRule(rule);
    }
  }
  // read each rule and verify

}




void Test::test()
{
  //Test::testVerification();
  FILE* fout = fopen("time.out", "w");
  struct timeval t, t1;
  gettimeofday(&t, NULL);
  testRocketfuelRouteViewsDataset();
  gettimeofday(&t1, NULL);
  fprintf(fout, "%f\n", t1.tv_sec - t.tv_sec + 1E-6 * (t1.tv_usec - t.tv_usec));
  fclose(fout);

}
