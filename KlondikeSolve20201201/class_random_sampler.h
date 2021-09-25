#pragma once

struct data_pair_s {
	unsigned long x1;
	unsigned long x2;
	unsigned long a1;
	unsigned long a2;
	struct data_pair_s* next;
};
typedef struct data_pair_s data_pair_t;

class RandomSampler {
public:
						RandomSampler(unsigned long x1, unsigned long x2, unsigned long x3, unsigned long x4, unsigned long x5);
						RandomSampler();
						~RandomSampler();
	void				m90setseeds(unsigned long x1, unsigned long x2 , unsigned long x3, unsigned long x4, unsigned long x5);
	void				m90getseeds(unsigned long* x1, unsigned long* x2, unsigned long* x3, unsigned long* x4, unsigned long* x5);
	char				m90randombit();
	unsigned long		m90random31();
	double				m90randomu();
	/*後付けした関数*/
	void				SeedQ();
	unsigned long		uniform(int num);

	unsigned long long used_num;

private:
	unsigned long omega[5];
};

class DRWS : public RandomSampler
{
public:
						DRWS(unsigned long x1, unsigned long x2, unsigned long x3, unsigned long x4, unsigned long x5);
						DRWS();
						~DRWS();
	//void				init_drws();				//コンストラクタに移動
	long				get_locmax();
	void				set_first_location();
	unsigned long		drws31();
	double				drwsu();
	void				mode_change(char m);
	char				get_mode();
	unsigned long		d_uniform(int num);

private:
	long location;
	long locmax;
	long locmaxmax = -1;
	data_pair_t random_list;
	data_pair_t* current_ptr;
	char mode = 'd';


	void end_drws();
	void set_locmaxmax(long n);
};