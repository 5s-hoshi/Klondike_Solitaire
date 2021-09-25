#include <iostream>
#include "class_random_sampler.h"
using namespace std;


#define LIMIT_30 0x3fffffff
#define LIMIT_31 0x7fffffff
#define CARRY_31 0x40000000
#define CARRY_32 0x80000000

//////////////////////////////
//		base class: RWS		//
//////////////////////////////

RandomSampler::RandomSampler()
{	
	SeedQ();
	used_num = 0;
}

RandomSampler::RandomSampler(unsigned long x1, unsigned long x2, unsigned long x3, unsigned long x4, unsigned long x5) 
{
	m90setseeds(x1, x2, x3, x4, x5);
	used_num = 0;
}

RandomSampler::~RandomSampler()
{
}
//m90の種指定、2進数30桁まで見ている
void RandomSampler::m90setseeds(unsigned long s0, unsigned long s1, unsigned long s2, unsigned long s3, unsigned long s4)
{
	omega[0] = s0 & LIMIT_30;
	omega[1] = s1 & LIMIT_30;
	omega[2] = s2 & LIMIT_30;
	omega[3] = s3 & LIMIT_30;
	omega[4] = s4 & LIMIT_30;
}

//種を取得、ポインタで得て代入するのは値が大きいから？
void RandomSampler::m90getseeds(unsigned long* sp0, unsigned long* sp1, unsigned long* sp2, unsigned long* sp3, unsigned long* sp4)
{
	*sp0 = omega[0];
	*sp1 = omega[1];
	*sp2 = omega[2];
	*sp3 = omega[3];
	*sp4 = omega[4];
}



char RandomSampler::m90randombit() {
	used_num++;
	static unsigned long alpha[5] = { 0x278dde6e, 0x17f4a7c1, 0x17ce7301, 0x205cedc8, 0x0d042089 }; //(sqrt(5)-1)/2 2^31未満
	char data_byte;
	union bitarray {
		unsigned long of_32bits;
		char of_8bits[4];
	} data_bitarray;
	int j;

	for (j = 4; j >= 1; ) {
		omega[j] += alpha[j];
		if (omega[j] & CARRY_31) {
			omega[j] &= LIMIT_30;
			omega[--j]++;
		}
		else j--;
	}
	omega[0] += alpha[0];
	omega[0] &= LIMIT_30;
	data_bitarray.of_32bits = omega[0] ^ omega[1] ^ omega[2];
	data_byte = data_bitarray.of_8bits[0] ^ data_bitarray.of_8bits[1] ^ data_bitarray.of_8bits[2] ^ data_bitarray.of_8bits[3];
	data_byte ^= (data_byte >> 4);
	data_byte ^= (data_byte >> 2);
	data_byte ^= (data_byte >> 1);
	return (1 & data_byte);
}


unsigned long RandomSampler::m90random31() {
	int j;
	unsigned long b = 0;
	for (j = 0; j < 30; j++) {
		b |= m90randombit();
		b <<= 1;
	}
	b |= m90randombit();
	return b;

}

double RandomSampler::m90randomu() {
	return (double)m90random31() / (double)CARRY_32;
}


//入力が整数かどうか、違ったら-1を返す
int NumCheck(char x)
{
	for (int i = 0; i < 10; i++)
	{
		if (x == '0' + i)
			return i;
	}
	return -1;
}
//入力がyかnか
bool CheckYorN()
{
	while (1)
	{
		string s;
		cin >> s;
		if (s == "y")
			return 1;
		else if (s == "n")
			return 0;
		else
			cout << "\"y\" または \"n\" を入力してください\n";
	}
}
//種指定をするかどうか、"n"なら 664426,5161592,7773372,84171419,1545を種として用いる
void RandomSampler::SeedQ()
{
	unsigned long x[5];
	cout << "種を指定しますか？<y/n>\n";
	bool seed_flag = CheckYorN();
	if (seed_flag)
	{
		cout << "種を5つ選択してください\n";
		for (int i = 0; i < 5; i++)
		{
			string str;
			unsigned long seed = 0;
			cout << i + 1 << "番目の種\n";
			while (1)
			{
				bool input_is_int = 1;
				cin >> str;
				int len = str.size();
				//1桁ずつポップさせて、数字なら入力そうでなければ打ち直し
				for (int j = 0; j < len; j++)
				{
					char input_ch = str.back();
					str.pop_back();
					int input_int = NumCheck(input_ch);
					//cout << j + 1 << "桁目 " << input_int << endl;


					if (input_int != -1)
					{
						seed += input_int * (unsigned long long)pow(10, j);
						//cout << "now seed is " << seed << endl;
					}
					else
					{
						cout << "整数値を入力してください\n";
						input_is_int = 0;
						break;
					}
				}

				if (!input_is_int)
					continue;
				else
					break;
			}
			x[i] = seed;
			seed = 0;
			cout << "omega" << i + 1 << " = " << x[i] << endl;
		}
	}
	else
	{
		x[0] = 664426; x[1] = 5161592; x[2] = 7773372; x[3] = 84171419; x[4] = 1545;
	}
	cout << "種は ";
	for (int i = 0; i < 5; i++)
		cout << x[i] << "  ";
	cout << "が選ばれました\n";
	m90setseeds(x[0], x[1], x[2], x[3], x[4]);
}



// num個の元の上の一様分布
unsigned long RandomSampler::uniform(int num)
{
	unsigned long prn = m90random31();
	while (1)
	{
		if (prn > 0xffffffff - (0xffffffff % num))
			prn = m90random31();
		else
			break;
	}
	return prn % num;
}



//////////////////////////////////////
//				DRWS				//
//////////////////////////////////////

DRWS::DRWS(unsigned long x1, unsigned long x2, unsigned long x3, unsigned long x4, unsigned long x5):RandomSampler(x1, x2, x3, x4, x5)
{

	locmax = -1;
	location = -1;
	random_list.next = 0;
	current_ptr = nullptr;
}
DRWS::DRWS():RandomSampler()
{
	locmax = -1;
	location = -1;
	random_list.next = 0;
	current_ptr = nullptr;
}
void DRWS::end_drws()
{
	data_pair_t* previous_ptr;
	if (random_list.next != 0)
	{
		current_ptr = random_list.next;
		previous_ptr = &random_list;
		while (current_ptr->next != 0) {
			previous_ptr = current_ptr;
			current_ptr = current_ptr->next;
		}
		delete current_ptr;
		previous_ptr->next = 0;
		end_drws();
	}
}
DRWS::~DRWS()
{
	// end_drws();
}
long DRWS::get_locmax()
{
	return locmax;
}
void DRWS::set_locmaxmax(long n) 
{
	locmaxmax = n;
}
void DRWS::set_first_location() 
{
	location = -1;
	current_ptr = &random_list;
}
unsigned long DRWS::drws31() 
{
	data_pair_t* p;

	location++;
	if ((locmaxmax > 0) && (location > locmaxmax)) return m90random31();

	if (location > locmax) {
		// p = (data_pair_t*)malloc(sizeof(data_pair_t));
		data_pair_t* p = new data_pair_t;
		if (p == 0) {
			cout << "!ERROR: we cannot prepare memory!\n";
			cin.get();
			return CARRY_32;
		}
		current_ptr->next = p;
		p->x1 = m90random31();
		p->x2 = m90random31();
		p->a1 = m90random31();
		p->a2 = m90random31();
		p->next = 0;
		locmax++;
	}
	current_ptr = current_ptr->next;
	current_ptr->x2 += current_ptr->a2;
	current_ptr->x1 += current_ptr->a1;
	if (current_ptr->x2 & CARRY_32) {
		current_ptr->x2 &= LIMIT_31;
		current_ptr->x1++;
	}
	return (current_ptr->x1 &= LIMIT_31);
}
double DRWS::drwsu()
{
	unsigned long drws31copy = drws31();
	if (drws31copy == CARRY_32) return -1.0;
	else return (double)drws31copy / (double)CARRY_32;
}
void DRWS::mode_change(char m)
{
	if (m == 'd' || m == 'i')
		mode = m;
	else
		cout << "mode variable must be 'd' (drws) or 'i' (rws i.e. iid-sampling.\n";
}
char DRWS::get_mode()
{
	return mode;
}
unsigned long DRWS::d_uniform(int num)
{
	unsigned long prn = drws31();
	while (1)
	{
		if (prn > 0xffffffff - (0xffffffff % num))
			prn = drws31();
		else
			break;
	}
	return prn % num;
}
