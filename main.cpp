#include "Calc.h"

/*
* Why there is no "using namespace std;"??
* Well, I try to make new habit.
* Using namespace std - yes, make code much compact, writing new parts of code become faster.
* But, the reason, why using is not so nice habit - in big projects this can lead to some namespace conflicts.
* For example, you make function "sort" and with using namespace std - it's become unclear - where std::sort and where your sort.
* Yes, sure - this project not that large - but, again - just trying make new habit.
*/



int Calc::FindPrioOp(std::string* query)
{
	size_t op_position = std::string::npos,
		same_prio = std::string::npos;

	if ((op_position = query->find("^")) != std::string::npos)
		return op_position;

	op_position = query->find("*");
	same_prio = query->find("/");
	if (op_position != std::string::npos || same_prio != std::string::npos)
		return op_position < same_prio ? op_position : same_prio;

	op_position = query->find("+");
	same_prio = query->find("-");
	if (op_position != std::string::npos || same_prio != std::string::npos)
		return op_position < same_prio ? op_position : same_prio;
	return std::string::npos;
}

void Calc::PerformOp(std::string* op, std::string* l_value, std::string* r_value, std::string* res)
{
	if (op->find("^") != std::string::npos) {
		*res = std::to_string(pow(std::stof(*l_value), std::stof(*r_value)));
	}
	else if (op->find("*") != std::string::npos) {
		*res = std::to_string(std::stof(*l_value) * std::stof(*r_value));
	}
	else if (op->find("/") != std::string::npos) {
		float fr_value = std::stof(*r_value);
		if (fr_value == 0.0) {
			m_status = Calc::DivByZero;
		}
		*res = std::to_string(std::stof(*l_value) / fr_value);
	}
	else if (op->find("+") != std::string::npos) {
		*res = std::to_string(std::stof(*l_value) + std::stof(*r_value));
	}
	else if (op->find("-") != std::string::npos) {
		*res = std::to_string(std::stof(*l_value) - std::stof(*r_value));
	}
	if (res->at(res->find_last_not_of('0')) == '.')
		res->erase(res->find_last_not_of('0'), std::string::npos);
	else
		res->erase(res->find_last_not_of('0') + 1, std::string::npos);
	return;
}

void Calc::solve(std::string* query)
{

	/**
	* First - we need to solve equations in brackets.
	* Keep in mind, that sqrt(x) - here brackets can't be deleted.
	* In general - solve, until in brackets doesn't stay only number - then exit sub solving and remove brackets.
	*
	* Problem is to find – where zero-level bracket open and where it’s closing pair. We can’t just take first bracket and last bracket:
	* For example, we have 1*(3+4)-(5+6). If we take first ‘(‘ and last ‘)’, we grab (3+4)-(5+6), so we break operation priority rules.
	* Also, we can’t take first ‘(‘ and first ‘)’. Just take a look:
	* 1+(2+(3+4)*5) – we grab (2+(3+4), unbalanced brackets.

	* My solution is following:
	* First, I check – is in query any ‘(‘ and where is it. If we still have brackets:
	* 1. Set counter for depth level to 0, maximal depth to 0, 
	*	 initialize empty vector of pair <size_t, size_t> - in this vector we will store location on opening bracket and closing bracket.
	* 2. If we found ‘(‘ – depth_level++, check – is it new max level of depth or not. If it is – clear vector with brackets location. 
	*	 If we on the max level of	depth – we add in vector new element and save position of opening bracket.
	*	 If we found ‘)’ – depth_ depth_level --, if we on max level of depth – add position of closing bracket to the last element of vector.
	* 3. If depth_level equal 0 – break, we don’t want to “explore” another brackets.
	* 4. Until vector with brackets positions not empty – trim from the beginning equations in brackets, call Calc::solve, receive result, 
	*	 check – maybe before		brackets “sqrt” operation – calculate sqrt (n) and replace result, if not – replace only (n) with n. 
	*	 Remove first element and change position of next brackets (since we replace some equation with calculation result, 
	*	 positions of next brackets can be changed).
	*5. When vector become empty, check again, maybe in equation still exist any ‘(‘, case we reduce only one level on depth. 
	*	Repeat until we didn’t remove all  brackets.
	**/
	while (query->find('(') != std::string::npos) {
		std::vector <std::pair<size_t, size_t>> brackets_depth;
		int depth_level = 0, max_depth = 0;
		for (std::string::size_type i = query->find('('); i < query->size(); ++i) {
			if (query->at(i) == '(') {
				++depth_level;
				if (depth_level > max_depth) {
					max_depth = depth_level;
					brackets_depth.clear();
				}
				if (depth_level == max_depth) {
					brackets_depth.push_back(std::pair<size_t, size_t>(i, 0));
				}
			}
			if (query->at(i) == ')') {
				if (depth_level == max_depth)
					brackets_depth.back().second = i;
				--depth_level;
			}

			if (depth_level == 0 && !brackets_depth.empty())
				break;
		}


		while (!brackets_depth.empty()) {

			std::string brackets_query = query->substr(brackets_depth.front().first + 1, brackets_depth.front().second - brackets_depth.front().first - 1);

			/*
			* Probably can be handy to save length before calculation, since changing of length also effect on position of high - level brackets.
			* Also, can be useful in replace function - store integer can be more compact, than another string.
			* +2 - for '(' and ')'
			*/
			int old_len = brackets_query.length() + 2;
			solve(&brackets_query);
			if (m_status != Calc::NoError)
				return;

			/*
			* Now we can replace equation in brackets with result, stored in brackets_qiery, since (n)=n.
			* But, before do it - we need to check - maybe before brackets sqr[t] operation.
			* If we here remove brackets - operation become invalid. So instead replace brackets with in_brackets equation, we do this operation,
			* since, luckily, it's have highest priority.
			*/
			if ((brackets_depth.front().first != 0) && (query->at(brackets_depth.front().first - 1) == 't')) {

				float test_val = std::stof(brackets_query);
				if (test_val < 0) {
					m_status = Calc::NegativeSqrt;
					return;
				}
				std::string res = std::to_string(sqrt(std::stof(brackets_query)));
				auto dbg = brackets_query.length();
				//brackets_depth.front().first-4 - where [sqrt(] starts; 6+brackets_query.length() - [sqrt()+in_brackets]
				query->replace(brackets_depth.front().first - 4, 6 + brackets_query.length(), res);
				old_len -= (res.length() - 4); //-4 - for sqrt. '(' and ')' noticed in old_len 
			}
			else {
				query->replace(brackets_depth.front().first, old_len, brackets_query);	//2 - to count '(' and ')' in length of replacement
				old_len -= brackets_query.length();
			}
			brackets_depth.erase(brackets_depth.begin());
			for (int i = 0; i < brackets_depth.size(); i++) {
				brackets_depth.at(i).first -= old_len;
				brackets_depth.at(i).second -= old_len;
			}

			if (m_trace)
				trace(*query);
		}
	}

	/*
	* Now we should be free from brackets.
	* Time to make some calculations, until in query string left only number, that will be our solution
	*/
	int pos;
	while ((pos = FindPrioOp(query)) != -1) {
		std::string l_value, r_value, op, res;
		for (int i = pos - 1; i >= 0; i--) {
			if ((query->at(i) >= '0' && query->at(i) <= '9') || query->at(i) == '.')
				l_value += query->at(i);
			else
				break;
		}
		std::reverse(l_value.begin(), l_value.end());

		for (int i = pos + 1; i < query->size(); i++) {
			if ((query->at(i) >= '0' && query->at(i) <= '9') || query->at(i) == '.')
				r_value += query->at(i);
			else
				break;
		}
		op = query->at(pos);
		PerformOp(&op, &l_value, &r_value, &res);
		if (m_status != Calc::NoError)
			return;
		query->replace(pos - l_value.size(), l_value.size() + op.size() + r_value.size(), res);
	}
}

void Calc::format(std::string* query) {
	size_t pos = -1;
	while ((pos = m_query.find(' ')) != std::string::npos) {
		query->erase(pos, 1);
	}
	while ((pos = m_query.find(',')) != std::string::npos) {
		query->at(pos) = '.';
	}
	while ((pos = m_query.find('\\')) != std::string::npos) {
		query->at(pos) = '/';
	}
	if ((pos = m_query.find('=')) != std::string::npos) {
		query->erase(pos, 1);
	}
}
void Calc::clear() {
	HeaderMess();
	m_query = "";
	m_status = Calc::NoError;
}

Calc::Calc() { clear(); }

void Calc::validate()
{
	if ((std::count(m_query.begin(), m_query.end(), '(') - std::count(m_query.begin(), m_query.end(), ')')) != 0) {
		std::cout << std::endl << error_mess("Brackets mismatch. Check, where you didn't close bracket bracket or place extra closing bracket");
		m_status = Calc::ValidationError;
	}


	/**
	* If input was from console - this validation is extra-operation. But we need it, since also have input with std::string.
	* And yes, we need input with std::string - for debugging and test purpose.
	**/
	std::string allowed_symbols = "1234567890"
		"-+\\/*"
		"sqrt"
		"().,=^"
		"\b ";	//Backspace, spacebar (to make it looks more pretty, why not)
	if (m_query.find_first_not_of(allowed_symbols) != std::string::npos) {
		std::cout << std::endl << error_mess("Unallowed characters in the query");
		m_status = Calc::ValidationError;
	}
	if (m_query.find_first_of("sqrt") != std::string::npos && m_query.find("sqrt") == std::string::npos) {
		std::cout << std::endl << error_mess("Unallowed operation in the query (deffenetly, sqrt wrong spell");
		m_status = Calc::ValidationError;
	}
}

void Calc::GetAnswer() {
	format(&m_query);
	validate();
	if (m_status == Calc::ValidationError)
		return;

	solve(&m_query);
	switch (m_status) {
	case Calc::NoError:
		if (m_trace)	std::cout << std::endl;
		std::cout << m_query << std::endl;
		break;
	case Calc::DivByZero:
		std::cout << error_mess("Division by zero");
		break;
	case Calc::NegativeSqrt:
		std::cout << error_mess("Extracting the root of a negative number");
		break;
	}
}

void Calc::input()
{
	char c = ' ';
	std::string allowed_symbols = "1234567890"
		"-+\\/*"
		"sqrt"
		"().,=^"
		"\b\n\r ";	//Backspace, spacebar (to make it looks more pretty, why not)
	std::string sqrt_check = "sqrt";
	int sqrt_check_pos = std::string::npos;

	while (c != '=')
	{
		c = _getch();
		/**
		 * I want to mark mistakes during input. If user used unallowed symbol - stop inputting and wait until user will not delete this symbol.
		 **/
		if (allowed_symbols.find(c) == std::string::npos || (sqrt_check_pos != std::string::npos && c != sqrt_check[sqrt_check_pos]))
		{
			std::cout << warn_symbol(c);
			while (c != '\b') {
				c = _getch();
			}
			cls();
			HeaderMess();
			std::cout << m_query;
		}
		else
		{
			if (c == '\b') {
				if (m_query.length() == 0)
					continue;
				if (sqrt_check_pos != -1)
					sqrt_check_pos--;

				m_query = m_query.substr(0, m_query.end() - m_query.begin() - 1);
			}
			else {
				if ((sqrt_check_pos = sqrt_check.find(c)) != std::string::npos)
					sqrt_check_pos++;
				if (sqrt_check_pos == sqrt_check.length())
					sqrt_check_pos = std::string::npos;
				if (c == '\n' || c == '\r') c = '=';
				m_query += c;
			}
			cls();
			HeaderMess();
			std::cout << m_query;
		}
	}

	GetAnswer();
}
void Calc::input(std::string query) {
	m_query = query;
	GetAnswer();
}
void Calc::trace_res(bool val) { m_trace = val; }

int main()
{
	Calc calc;
	char c;

	do {
		calc.trace_res(true);
		calc.input();
		std::cout << "Try again? y\\n > ";
		c = _getch();
		cls();
		std::cout << "< ";
		calc.clear();
	} while (c == 'y' || c == 'Y');
}
