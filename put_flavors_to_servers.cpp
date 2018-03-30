#include "Server.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

//������ĵ���Դ��CPU��Ϊtrue��������ĵ���Դ���ڴ���ΪFalse
#define CPU true 
#define MEM false

using namespace std;

///ʹ��ģ���˻��㷨����ѵ���������÷�ʽ
///���������
///map_predict_num_flavors����һ��Ԥ������ĸ��������������key����������ƣ�value�����������
///map_flavor_cpu_mem�����������������������ݣ�keyΪ��������ƣ�value����������ͣ�����name��cpu��mem�ֶΣ�
///server_mem && server_cpu����������ķ�����������CPU���ڴ��С
///CPUorMEM����ʹCPU��������߻���ʹ�ڴ����������
///���������
///res_servers������м���������ŷ��������������ŷ�ʽ����ͨ����Աflavors����ÿ���������д�ŵ������
vector<Server> put_flavors_to_servers(unordered_map<string, int> map_predict_num_flavors, 
	unordered_map<string, Flavor> map_flavor_cpu_mem,int server_mem, int server_cpu, bool CPUorMEM) {

	vector<Flavor> vec_flavors;    //vector���ڴ������Ԥ�������flavor
	for (auto element : map_predict_num_flavors) {
		//��Ԥ���������������������뵽vec_flavors��
		while (element.second-- != 0) {
			vec_flavors.push_back(map_flavor_cpu_mem[element.first]);
		}
	}

	//=========================================================================
	//ģ���˻��㷨�����Ž�
	double min_server = vec_flavors.size() + 1;
	vector<Server> res_servers;  //���ڴ����ý����������ʹ���������٣�
	double T = 100.0;  //ģ���˻��ʼ�¶�
	double Tmin = 1;   //ģ���˻���ֹ�¶�
	double r = 0.9999; //�¶��½�ϵ��
	vector<int> dice;  //���ӣ�ÿ�����Ͷ����ȡvectorǰ����������Ϊÿ���˻���Ҫ����˳��������
	for (int i = 0; i < vec_flavors.size(); i++) {
		dice.push_back(i);
	}
	while (T > Tmin) {
		//Ͷ�����ӣ���vectorǰ������Ϊ3��9�����vec_flavors[3]��vec_flavors[9]���н�����Ϊ�µ�flavors˳��
		std::random_shuffle(dice.begin(), dice.end());
		auto new_vec_flavors = vec_flavors;
		std::swap(new_vec_flavors[dice[0]], new_vec_flavors[dice[1]]);

		//����һ�������������������Լ��뵽��������

		//��ʹ��һ�����������ڷ��������
		vector<Server> servers;
		Server firstserver(server_mem, server_cpu);
		servers.push_back(firstserver);  
		
		//�����������Ҫ�߼�
		//�����ǰ���з��������Ų�������������½�һ�����������ڴ��
		for (auto element : new_vec_flavors) {
			auto iter = servers.begin();
			for (; iter != servers.end(); ++iter) {
				if (iter->put_flavor(element)) {
					break;
				}
			}
			if (iter == servers.end()) {
				Server newserver(server_mem, server_cpu);
				newserver.put_flavor(element);
				servers.push_back(newserver);
			}
		}

		//���㱾�η���������ķѷ��������۷���(double��)
		//���ʹ����N������������ǰN-1�����������׷���Ϊ1����N������������Ϊ��Դ������
		//ģ���˻���ǵõ�ȡ�÷�����С�ķ��÷�ʽ
		double server_num;
		//������Ŀ����CPU����MEM����Ҫ�ֿ����ۣ���Դ�����ʼ��㷽����ͬ
		if (CPUorMEM == CPU)
			server_num = servers.size() - 1 + servers.rbegin()->get_cpu_usage_rate();
		else
			server_num = servers.size() - 1 + servers.rbegin()->get_mem_usage_rate();
		//����������ͣ��򱣴���
		if (server_num < min_server) {
			min_server = server_num;
			res_servers = servers;
			vec_flavors = new_vec_flavors;
		}
		//����������ߣ�����һ�����ʱ���������ֹ�Ż�����ֲ����Ž�
		else {
			if (exp((min_server - server_num) / T) > rand() / RAND_MAX) {
				min_server = server_num;
				res_servers = servers;
				vec_flavors = new_vec_flavors;
			}
		}
		T = r * T;  //һ��ѭ���������¶Ƚ���
	}
	return res_servers;
}

int main() {
	//������������
	//Ԥ������ĸ������������
	unordered_map<string, int> map_predict_num_flavors;
	map_predict_num_flavors["flavor1"] = 30;
	map_predict_num_flavors["flavor2"] = 40;
	map_predict_num_flavors["flavor3"] = 20;
	map_predict_num_flavors["flavor4"] = 10;
	map_predict_num_flavors["flavor5"] = 25;

	//�������������
	unordered_map<string, Flavor> map_flavor_cpu_mem;
	map_flavor_cpu_mem["flavor1"] = Flavor("flavor1", 1, 1);
	map_flavor_cpu_mem["flavor2"] = Flavor("flavor2", 1, 2);
	map_flavor_cpu_mem["flavor3"] = Flavor("flavor3", 1, 4);
	map_flavor_cpu_mem["flavor4"] = Flavor("flavor4", 2, 2);
	map_flavor_cpu_mem["flavor5"] = Flavor("flavor5", 2, 4);

	//��������Դ�����Ϣ
	int server_cpu = 56;
	int server_mem = 128;
	bool CPUorMEM = CPU;
	//����ģ���˻��㷨�ҵ����ŷ��÷���
	vector<Server> servers = put_flavors_to_servers(map_predict_num_flavors, map_flavor_cpu_mem, server_cpu, server_mem, CPUorMEM);
	//�����������������ֱ��������Щ�����
	int server_count = 1;
	for (auto element : servers) {
		cout << server_count++ << " ";
		for (auto flavor : element.flavors) {
			std::cout << flavor.name << " " << 1 << " ";
		}
		cout << endl;
	}
	return 0;
}