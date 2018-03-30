#include "Server.h"

///Server���캯��������Ϊ�ڴ��С��CPU��С
Server::Server(int mem, int cpu) :total_mem(mem), total_cpu(cpu) {
	free_cpu = cpu;  //��ʼ��ʱʣ��CPU������CPU
	free_mem = mem;  //��ʼ��ʱʣ���ڴ�������ڴ�
}
///�������������������Ϊ��������󣬷���ֵΪ�Ƿ���óɹ�
///���ȼ��ʣ��CPU���ڴ��Ƿ��㹻���ø������
///����ܹ�����������������������������������·����������ڴ�Ϳ���CPU��������true
///���ʣ���ڴ��CPU�����Է��¸���������򷵻�false
bool Server::put_flavor(Flavor flavor) {
	if (free_cpu >= flavor.cpu && free_mem >= flavor.mem) {
		free_cpu -= flavor.cpu;
		free_mem -= flavor.mem;
		flavors.push_back(flavor);
		return true;
	}
	return false;
}

///��ȡ������CPUʹ����
double Server::get_cpu_usage_rate() {
	return free_cpu / static_cast<double>(total_cpu);
}
///��ȡ�������ڴ�ʹ����
double Server::get_mem_usage_rate() {
	return free_mem / static_cast<double>(total_mem);
}