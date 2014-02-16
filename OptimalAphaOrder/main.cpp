#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <chrono>
#include <random>
#include <future>
#include <assert.h>

typedef std::map<char, int> alphabet_t;
typedef std::map<int, char> inversed_alphabet_t;

alphabet_t base_alphabet()
{
	alphabet_t alpha;

	for(int i = 0; i < 26; ++i)
		alpha.insert(std::make_pair(static_cast<char>('A' + i), i));

	return alpha;
}

alphabet_t reversed_alphabet()
{
	alphabet_t alpha;

	for(int i = 25; i >=0; i--)
		alpha.insert(std::make_pair(static_cast<char>('Z' - i), i));

	return alpha;
}

inversed_alphabet_t inverse_alphabet_map(alphabet_t const & alphabet)
{
	inversed_alphabet_t inversedalpha;

	for(auto const & elem : alphabet)
		inversedalpha.insert(std::make_pair(elem.second, elem.first));

	return inversedalpha;
}

std::string alphabet_string(alphabet_t const & alphabet)
{
	std::string alphabet_string(alphabet.size(), ' ');
	auto reversed = inverse_alphabet_map(alphabet);

	std::transform(
		begin(reversed), end(reversed), 
		begin(alphabet_string),
		[&](std::pair<int, char> const & elem) {return elem.second;}
	);

	return alphabet_string;
}

bool is_ordered(std::string const & word, alphabet_t const & alphabet)
{
	if(word.size() < 2) return true;

	for(size_t i = 1; i < word.size(); ++i)
	{
		auto lpos1 = alphabet.find(word[i]);
		auto lpos2 = alphabet.find(word[i-1]);
		if(lpos1->second < lpos2->second)
			return false;
	}

	return true;
}

int alphabet_score(std::vector<std::string> const & words, alphabet_t const & alphabet)
{
	return std::count_if(
		begin(words), end(words), 
		[&alphabet](std::string const & word) {return is_ordered(word, alphabet);});
}

void swap_letter(alphabet_t & alphabet, char const l1, char const l2)
{
	std::swap(alphabet[l1], alphabet[l2]);
}

void alter_alphabet(alphabet_t & alphabet, std::mt19937 & generator, std::uniform_int_distribution<int> & uniform_distribution)
{
	auto l1 = 'A' + uniform_distribution(generator);
	auto l2 = 0;
	do {
		l2 = 'A' + uniform_distribution(generator);
	} while(l2 == l1);

	swap_letter(alphabet, l1, l2);
}

void shuffle_alphabet(alphabet_t & alphabet)
{
	std::vector<alphabet_t::mapped_type> values(alphabet.size());
	std::transform(
		begin(alphabet), end(alphabet),
		begin(values),
		[&](alphabet_t::value_type const & elem) {return elem.second;});

	std::random_shuffle(begin(values), end(values));

	auto crtvalue = begin(values);
	for(auto & elem : alphabet)
	{
		elem.second = *crtvalue++;
	}
}

std::vector<std::string> read_words()
{
	std::ifstream file("words.txt");
	std::istream_iterator<std::string> start(file), end;
	std::vector<std::string> words(start, end);
	return words;
}

std::pair<int, std::string> find_optimal_alphabet(std::vector<std::string> const & words, int const timeout)
{
	std::mt19937 rndgenerator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distribution(0,25);
		
	auto bestscore = std::make_pair(0, std::string(""));
	auto alpha = base_alphabet();
	int iteration = 0;

	auto start_time = std::chrono::system_clock::now();

	while(true)
	{
		auto crt_time = std::chrono::system_clock::now();
		if(std::chrono::duration_cast<std::chrono::seconds>(crt_time-start_time) > std::chrono::seconds(timeout))
			break;

		auto score = alphabet_score(words, alpha);

		if(score > bestscore.first)
		{
			bestscore.first = score;
			bestscore.second = alphabet_string(alpha);
			std::cout << score << " \t" << alphabet_string(alpha) << "  iteration=" << iteration << std::endl;
		}

		alter_alphabet(alpha, rndgenerator, distribution);
		iteration++;

		if((iteration % 1000) == 0)
			alter_alphabet(alpha, rndgenerator, distribution);
		if((iteration % 5000) == 0)
			alter_alphabet(alpha, rndgenerator, distribution);
		if((iteration % 50000) == 0)
			shuffle_alphabet(alpha);
	}

	return bestscore;
}

std::pair<int, std::string> find_optimal_alphabet_async(std::vector<std::string> const & words, int const timeout, int const seed)
{
	std::mt19937 rndgenerator(seed);
	std::uniform_int_distribution<int> distribution(0,25);

	auto bestscore = std::make_pair<int, std::string>(0, std::string(""));
	auto alpha = base_alphabet();
	shuffle_alphabet(alpha);

	int iteration = 0;

	auto start_time = std::chrono::system_clock::now();

	while(true)
	{
		auto crt_time = std::chrono::system_clock::now();
		if(std::chrono::duration_cast<std::chrono::seconds>(crt_time-start_time) > std::chrono::seconds(timeout))
			break;

		auto score = alphabet_score(words, alpha);

		if(score > bestscore.first)
		{
			bestscore.first = score;
			bestscore.second = alphabet_string(alpha);
		}

		alter_alphabet(alpha, rndgenerator, distribution);
		iteration++;

		if((iteration % 1000) == 0)
			alter_alphabet(alpha, rndgenerator, distribution);
		if((iteration % 5000) == 0)
			alter_alphabet(alpha, rndgenerator, distribution);
		if((iteration % 50000) == 0)
			shuffle_alphabet(alpha);
	}

	return bestscore;
}

void find_optimal_alphabet_sequential(std::vector<std::string> const & words, int const timeout)
{
	auto bestscore = find_optimal_alphabet(words, timeout);

	std::cout << "best score:" << std::endl;
	std::cout << bestscore.first << " \t" << bestscore.second << std::endl;
}

void find_optimal_alphabet_parallel(std::vector<std::string> const & words, int const timeout, int const taskscount)
{
	std::vector<std::future<std::pair<int, std::string>>> futures(taskscount);

	unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	unsigned int index = 0;
	for(auto & f : futures)
	{
		f = std::async(std::launch::async, [&](){return find_optimal_alphabet_async(words, timeout, seed + index++);});
	}

	auto bestscore = std::make_pair<int, std::string>(0, std::string(""));
	for(auto & f : futures)
	{
		auto score = f.get();
		std::cout << score.first << " \t" << score.second << std::endl;
		if(score.first > bestscore.first)
			bestscore = score;
	}

	std::cout << "best score:" << std::endl;
	std::cout << bestscore.first << " \t" << bestscore.second << std::endl;
}

void test_alphabet_string()
{
	auto alpha1 = base_alphabet();
	auto alphastr1 = alphabet_string(alpha1);
	assert(alphastr1 == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	auto alpha2 = reversed_alphabet();
	auto alphastr2 = alphabet_string(alpha2);
	assert(alphastr2 == "ZYXWVUTSRQPONMLKJIHGFEDCBA");
}

void test_swap_letter()
{
	auto alpha1 = base_alphabet();
	swap_letter(alpha1, 'A', 'B');
	auto alphastr1 = alphabet_string(alpha1);
	assert(alphastr1 == "BACDEFGHIJKLMNOPQRSTUVWXYZ");

	auto alpha2 = base_alphabet();
	swap_letter(alpha2, 'Y', 'Z');
	auto alphastr2 = alphabet_string(alpha2);
	assert(alphastr2 == "ABCDEFGHIJKLMNOPQRSTUVWXZY");

	auto alpha3 = base_alphabet();
	swap_letter(alpha3, 'A', 'Z');
	auto alphastr3 = alphabet_string(alpha3);
	assert(alphastr3 == "ZBCDEFGHIJKLMNOPQRSTUVWXYA");

	auto alpha4 = base_alphabet();
	swap_letter(alpha4, 'I', 'O');
	auto alphastr4 = alphabet_string(alpha4);
	assert(alphastr4 == "ABCDEFGHOJKLMNIPQRSTUVWXYZ");
}

void test_is_ordered()
{
	auto alpha1 = base_alphabet();
	assert(true == is_ordered("ABCD", alpha1));
	assert(true == is_ordered("AABCDXYZ", alpha1));
	assert(false == is_ordered("AACB", alpha1));

	swap_letter(alpha1, 'A', 'B');
	assert(false == is_ordered("ABCD", alpha1));
	assert(false == is_ordered("AABCDXYZ", alpha1));
	assert(true == is_ordered("BAC", alpha1));
	assert(true == is_ordered("BBAAC", alpha1));
	assert(false == is_ordered("BCA", alpha1));
}

void test_alphabet_score()
{
	std::string arrwords[] = {"THIS", "IS", "A", "SIMPLE", "ALPHABET", "SCORE", "TEST"};
	std::vector<std::string> words(begin(arrwords), end(arrwords));
	assert(2 == alphabet_score(words, base_alphabet()));

	assert(860 == alphabet_score(read_words(), base_alphabet()));
}

int main()
{
	//test_alphabet_string();
	//test_swap_letter();
	//test_is_ordered();
	//test_alphabet_score();

	//find_optimal_alphabet_sequential(read_words(), 60);

	find_optimal_alphabet_parallel(read_words(), 60, 8);

	std::cin.get();
	return 0;
}