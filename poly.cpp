#define _CRT_SECURE_NO_WARNINGS_
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <string>
#include <string.h>
#include <set>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Backends/SFML.hpp>
#include <TGUI/AllWidgets.hpp>

#define ll long long
#define mp make_pair
#define pass (void)0

using namespace std;

set<char> se = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

struct mono {
	int ratio;
	int degree;
	mono* next;
};

struct node {
	int number;
	mono* poly = nullptr;
	mono* endP = nullptr;
	node* next = nullptr;
};

node* L = nullptr, * endL = L;

void char_deleter(string& s, char what) {
	string s2 = "";
	for (auto& i : s) {
		if (i != what)
			s2.push_back(i);
	}
	s = s2;
}

// vector<degree, ratio>


vector<pair<int, int> > poly_parser(string s) {
	vector<pair<int, int> > v;
	int sign = 1;
	string number = "";
	bool degree = false;
	for (int i = 0; i < s.size() + 1; ++i) {
		if (i == s.size() || (s[i] == '+' || s[i] == '-') && i) {
			if (!degree) {
				if (!number.size())
					v.push_back(mp(0, sign));
				else if (stoi(number)) {
					v.push_back(mp(0, sign * stoi(number)));
				}
				else
					v.push_back(mp(0, 0));
				number = "";
			}
			else {
				if (!v.back().second)
					v.pop_back();
				else {
					if (!number.size())
						v.back().first = -1;
					else
						v.back().first = -stoi(number);
				}
				number = "";
			}
			if (s[i] == '+')
				sign = 1;
			else
				sign = -1;
		}
		if (s[i] == '+') {
			sign = 1;
			number = "";
			degree = false;
		}
		else if (s[i] == '-') {
			sign = -1;
			number = "";
			degree = false;
		}
		else if (s[i] == 'x') {
			if (!number.size())
				v.push_back(mp(0, sign));
			else if (stoi(number)) {
				v.push_back(mp(0, sign * stoi(number)));
			}
			else
				v.push_back(mp(0, 0));
			number = "";
			degree = true;
		}
		else if (s[i] == '^') {
			pass;
		}
		else {
			number.push_back(s[i]);
		}
	}
	sort(v.begin(), v.end());
	if (!v.size())
		v.push_back(mp(0, 0));
	return v;
}


// приводит подобные и добавляет в базу
node* poly_sum_adder(vector<pair<int, int> > v, node* &iter) {
	int sum = 0;
	if (iter == endL) {
		if (endL == nullptr) {
			iter = new node;
			L = iter;
			iter->number = 1;
		}
		else {
			iter->next = new node;
			iter->next->number = iter->number + 1;
			iter = iter->next;
		}
	}
	else {
		iter = new node;
	}
	pair<int, int> prev = v[0];
	for (int i = 0; i < v.size() + 1; ++i) {
		if (i < v.size() && v[i].first == prev.first)
			sum += v[i].second;
		else {
			if (sum) {
				if (iter->poly == nullptr) {
					iter->poly = new mono;
					iter->poly->next = nullptr;
					iter->endP = iter->poly;
					iter->poly->ratio = sum;
					iter->poly->degree = -prev.first;
				}
				else {
					iter->endP->next = new mono;
					iter->endP = iter->endP->next;
					iter->endP->next = nullptr;
					iter->endP->ratio = sum;
					iter->endP->degree = -prev.first;
				}
			}
			if (i < v.size())
				sum = v[i].second;
		}
		if (i < v.size())
			prev = v[i];
	}
	if (iter->poly == nullptr) {
		iter->poly = new mono;
		iter->poly->next = nullptr;
		iter->endP = iter->poly;
		iter->poly->ratio = 0;
		iter->poly->degree = 0;
	}
	if (iter != endL)
		return iter;
	else {
		return nullptr;
	}
}

void poly_adder(mono* q) {
	endL->next = new node;
	endL->next->number = endL->number + 1;
	endL = endL->next;
	endL->poly = q;
	mono* tmp = endL->poly;
	while (tmp->next != nullptr) {
		tmp = tmp->next;
	}
	endL->endP = tmp;
}

void poly_deleter(int number) {
	node* tmp = L, *buf = nullptr;
	while (tmp->number != number && tmp->next != nullptr) {
		if (tmp->next->number == number)
			buf = tmp;
		tmp = tmp->next;
	}
	if (tmp->number != number)
		return;
	if (tmp->next != nullptr) {
		node* q = tmp->next;
		*tmp = *(tmp->next);
		delete(q);
		tmp->number -= 1;
		while (tmp->next != nullptr) {
			tmp = tmp->next;
			tmp->number -= 1;
		}
	}
	else {
		mono* m = tmp->poly;
		while (m->next != nullptr) {
			mono* a = m;
			m = m->next;
			delete(a);
		}
		delete(m);
		if (tmp->number == 1) {
			delete(tmp);
			L = nullptr;
		}
		else {
			delete(tmp);
		}
		if (buf != nullptr) {
			buf->next = nullptr;
		}
	}
	endL = L;
	if (endL != nullptr) {
		while (endL->next != nullptr)
			endL = endL->next;
		endL->next = nullptr;
	}
}


int state_machine(string& s) {
	string prev = "beg";
	int res = 1;
	for (auto& i : s) {
		if (prev == "beg") {
			if (i == '+' || i == '-') {
				prev = "p";
			}
			else if (i == '0') {
				prev = "z";
			}
			else if (se.find(i) != se.end()) {
				prev = "n";
			}
			else if (i == 'x') {
				prev = "x";
			}
			else {
				return 0;
			}
		}
		else if (prev == "n") {
			if (i == '+' || i == '-') {
				prev = "p";
			}
			else if (i == '*') {
				prev = "m";
			}
			else if (i == 'x') {
				prev = "x";
			}
			else if (i == '0' || se.find(i) != se.end()) {
				prev = "n";
			}
			else {
				return 0;
			}
		}
		else if (prev == "z") {
			if (i == '+' || i == '-') {
				prev = "p";
			}
			else if (i == '*') {
				prev = "m";
			}
			else if (i == 'x') {
				prev = "x";
			}
			else {
				return 0;
			}
		}
		else if (prev == "p") {
			if (i == '0') {
				prev = "z";
			}
			else if (i == 'x') {
				prev = "x";
			}
			else if (se.find(i) != se.end()) {
				prev = "n";
			}
			else {
				return 0;
			}
		}
		else if (prev == "m") {
			if (i == 'x') {
				prev = "x";
			}
			else {
				return 0;
			}
		}
		else if (prev == "x") {
			if (i == '-' || i == '+') {
				prev = "p";
			}
			else if (i == '^') {
				prev = "d";
			}
			else {
				return 0;
			}
		}
		else if (prev == "d") {
			if (se.find(i) != se.end()) {
				prev = "n2";
			}
			else if (i == '0') {
				prev = "z2";
			}
			else {
				return 0;
			}
		}
		else if (prev == "z2") {
			if (i == '-' || i == '+') {
				prev = "p";
			}
			else {
				return 0;
			}
		}
		else if (prev == "n2") {
			if (se.find(i) != se.end() || i == '0'){
				prev = "n2";
			}
			else if (i == '-' || i == '+') {
				prev = "p";
			}
			else {
				return 0;
			}
		}
	}
	if (prev != "n" && prev != "z" && prev != "x" && prev != "n2" && prev != "z2")
		return 0;
	return 1;
}



pair<int, int> derivate_mono(int ratio, int degree, int cnt) {	//order - порядок производной, cnt - сколько раз осталось взять
	if (cnt == 0 || ratio == 0)
		return mp(ratio, degree);
	--cnt;
	return derivate_mono(ratio * degree, max(0, degree - 1), cnt);
}

node* derivate_poly(int number, int order) {
	if (number > endL->number) {
		return nullptr;
	}
	node* tmp = L;
	while (tmp->number < number)
		tmp = tmp->next;
	mono* res, *ind; // res - начало list производной данного многочлена, ind - итератор по list производной
	mono* q = tmp->poly; // итератор по данному многочлену
	pair<int, int> pa1 = derivate_mono(q->ratio, q->degree, order);
	res = new mono;
	res->ratio = pa1.first;
	res->degree = pa1.second;
	ind = res;
	while (q->next != nullptr) {
		q = q->next;
		pair<int, int> pa = derivate_mono(q->ratio, q->degree, order);
		ind->next = new mono;
		ind = ind->next;
		ind->next = nullptr;
		ind->ratio = pa.first;
		ind->degree = pa.second;
	}
	ind->next = nullptr;
	node* out = new node;
	out->poly = res;
	out->endP = q;

	mono* iter1 = out->poly;
	while (iter1->next != nullptr) {
		if (iter1->ratio == 0) {
			mono* q = iter1->next;
			if (iter1->next != nullptr) {
				*iter1 = *(iter1->next);
				delete(q);
			}
			else {
				iter1 = iter1->next;
			}
		}
		else
			iter1 = iter1->next;
	}
	if (iter1->ratio == 0) {
		mono* q = iter1->next;
		if (iter1->next != nullptr) {
			*iter1 = *(iter1->next);
			delete(q);
		}
		else {
			iter1 = iter1->next;
		}
	}
	iter1 = out->poly;
	while (iter1->next != nullptr)
		iter1 = iter1->next;
	out->endP = iter1;
	return out;
}



// vector<degree, ratio>
vector<pair<int, int> > multiply_poly(int first, int second) {
	vector<pair<int, int> > v;
	node* tmp = L; //итератор по внешнему list
	mono* ind1, *ind2;
	if (first > endL->number || second > endL->number)
		return v; // нужно поставить обработку ошибки
	while (tmp->number < min(first, second))
		tmp = tmp->next;
	ind1 = tmp->poly;
	while (tmp->number < max(first, second))
		tmp = tmp->next;
	ind2 = tmp->poly;
	mono* iter1 = ind1, * iter2 = ind2;
	while (iter1 != nullptr) {
		while (iter2 != nullptr) {
			v.push_back(mp(-(iter1->degree + iter2->degree), iter1->ratio * iter2->ratio));
			iter2 = iter2->next;
		}
		iter1 = iter1->next;
		iter2 = ind2;
	}
	sort(v.begin(), v.end());
	return v;
}


node* sum_poly(int first, int second) {
	node* res = new node;
	mono* endRes = nullptr;
	node* tmp = L; //итератор по внешнему list
	mono* ind1, * ind2;
	if (first > endL->number || second > endL->number)
		return nullptr; // нужно поставить обработку ошибки
	while (tmp->number < min(first, second))
		tmp = tmp->next;
	ind1 = tmp->poly;
	while (tmp->number < max(first, second))
		tmp = tmp->next;
	ind2 = tmp->poly;
	mono* iter1 = ind1, * iter2 = ind2;
	while (iter1 != nullptr || iter2 != nullptr) {
		if (endRes == nullptr) {
			res->poly = new mono;
			endRes = res->poly;
			endRes->next = nullptr;
		}
		else {
			endRes->next = new mono;
			endRes = endRes->next;
			endRes->next = nullptr;
		}
		if (iter1 == nullptr) {
			*endRes = *iter2;
			endRes->next = nullptr;
			iter2 = iter2->next;
		}
		else if (iter2 == nullptr) {
			*endRes = *iter1;
			endRes->next = nullptr;
			iter1 = iter1->next;
		}
		else {
			if (iter1->degree > iter2->degree) {
				*endRes = *iter1;
				endRes->next = nullptr;
				iter1 = iter1->next;
			}
			else if (iter1->degree < iter2->degree) {
				*endRes = *iter2;
				endRes->next = nullptr;
				iter2 = iter2->next;
			}
			else {
				*endRes = *iter1;
				endRes->ratio += iter2->ratio;
				endRes->next = nullptr;
				iter1 = iter1->next;
				iter2 = iter2->next;
			}
		}
	}
	iter1 = res->poly;
	while (iter1->next != nullptr) {     //////// ИСПРАВИТЬ ОШИБКУ (513 СТРОКА) PS Вроде пофиксил
		if (iter1->ratio == 0) {
			mono* q = iter1->next;
			*iter1 = *(iter1->next);
			delete(q);
		}
		else
			iter1 = iter1->next;
	}
	if (iter1->ratio == 0) {
		mono* q = iter1->next;
		*iter1 = *(iter1->next);
		delete(q);
	}
	iter1 = res->poly;
	while (iter1->next != nullptr)
		iter1 = iter1->next;
	res->endP = iter1;
	return res;
}

// спрашивает, нужно ли добавить и добавляет
void need_add(node* iter) {
	//cout << "Do you want to add this polynomial to data base? [Y/N]" << "\n";
	string s;
	//cin >> s;
	s = "Y";
	if (iter == nullptr)
		return;
	if (s == "Y" && iter != nullptr) {
		endL->next = iter;
		iter->next = nullptr;
		iter->number = endL->number + 1;
		endL = endL->next;
	}
	else {
		mono* tmp = iter->poly;
		delete(iter);
		mono* q;
		while (tmp->next != nullptr) {
			q = tmp;
			tmp = tmp->next;
			delete(q);
		}
		delete(tmp);
	}
}


void display_on_data(tgui::ListBox::Ptr listbox) {
	if (L == nullptr)
		L = endL;
	node* tmp = L;
	while (tmp->next != nullptr)
		tmp = tmp->next;
	string out = "";
	mono* q = tmp->poly;
	while (q != nullptr) {
		if (q->ratio != 0 && q->ratio != 1) {
			if (q != tmp->poly && q->ratio > 0) {
				out += "+";
			}
			if (q->ratio != -1)
				out += to_string(q->ratio);
			else
				out += "-";
		}
		else if (q->ratio == 1) {
			if (q != tmp->poly) {
				out += "+";
			}
			if (q->degree == 0)
				out += "1";
		}
		if (q->degree != 0)
			if (q->degree == 1)
				out += "X";
			else
				out += "X^" + to_string(q->degree);
		q = q->next;
	}
	if (out == "")
		out = "0";
	out = to_string(endL->number) + ")    " + out;
	listbox->addItem(out);
}

sf::RenderWindow window(sf::VideoMode(1200, 800), "TGUI window");

bool runExample(tgui::GuiBase& gui)
{
	try
	{
		tgui::Theme theme{ "C:/Users/MiNotebook/TGUI-0.9/themes/BabyBlue.txt" };

		auto listBox = tgui::ListBox::create();
		listBox->setRenderer(theme.getRenderer("ListBox"));
		listBox->setSize(900, 700);
		listBox->setItemHeight(44);
		listBox->setPosition(10, 10);
		listBox->setTextSize(25);

		gui.add(listBox);

		auto editBox = tgui::EditBox::create();
		editBox->setRenderer(theme.getRenderer("EditBox"));
		editBox->setSize(800, 50);
		editBox->setTextSize(18);
		editBox->setPosition(10, 740);
		editBox->setDefaultText("Type your polynomial here...");
		gui.add(editBox);

		auto button = tgui::Button::create();
		button->setRenderer(theme.getRenderer("Button"));
		button->setPosition(850, 740);
		button->setText("ADD");
		button->setTextSize(20);
		button->setSize(100, 50);
		gui.add(button);
		button->onPress([=] {
			string s = editBox->getText().toStdString();
			char_deleter(s, ' ');
			if (state_machine(s)) {
				char_deleter(s, '*');
				poly_sum_adder(poly_parser(s), endL);
				display_on_data(listBox);
			}
		});


		auto ex_but = tgui::Button::create();
		ex_but->setRenderer(theme.getRenderer("Button"));
		ex_but->setPosition(1080, 740);
		ex_but->setText("EXIT");
		ex_but->setTextSize(20);
		ex_but->setSize(100, 50);
		gui.add(ex_but);
		ex_but->onPress([=] {
			window.close();
		});


		auto saver = tgui::CheckBox::create();
		saver->setRenderer(theme.getRenderer("CheckBox"));
		saver->setPosition(950, 420);
		saver->setText("Save the result");
		saver->setTextSize(22);
		saver->setChecked(true);
		saver->setSize(50, 50);
		gui.add(saver);
		

		
		auto editBox1 = tgui::EditBox::create();
		editBox1->setRenderer(theme.getRenderer("EditBox"));
		editBox1->setSize(50, 50);
		editBox1->setTextSize(18);
		editBox1->setPosition(950, 20);
		editBox1->setDefaultText("N");
		gui.add(editBox1);

		auto editBox2 = tgui::EditBox::create();
		editBox2->setRenderer(theme.getRenderer("EditBox"));
		editBox2->setSize(50, 50);
		editBox2->setTextSize(18);
		editBox2->setPosition(1020, 20);
		editBox2->setDefaultText("N");
		gui.add(editBox2);

		auto button1 = tgui::Button::create();
		button1->setRenderer(theme.getRenderer("Button"));
		button1->setPosition(1080, 20);
		button1->setText("+");
		button1->setTextSize(20);
		button1->setSize(100, 50);
		gui.add(button1);
		button1->onPress([=] {
			int first, second;
			if (L != nullptr) {
				try {
					first = stoi(editBox1->getText().toStdString());
					second = stoi(editBox2->getText().toStdString());
					if (min(first, second) >= L->number && max(first, second) <= endL->number) {
						need_add(sum_poly(first, second));
						display_on_data(listBox);
					}
				}
				catch (exception e) {
					pass;
				}
			}
			else
				pass;
		});

		auto editBox3 = tgui::EditBox::create();
		editBox3->setRenderer(theme.getRenderer("EditBox"));
		editBox3->setSize(50, 50);
		editBox3->setTextSize(18);
		editBox3->setPosition(950, 120);
		editBox3->setDefaultText("N");
		gui.add(editBox3);


		auto editBox6 = tgui::EditBox::create();
		editBox6->setRenderer(theme.getRenderer("EditBox"));
		editBox6->setSize(50, 50);
		editBox6->setTextSize(18);
		editBox6->setPosition(1020, 120);
		editBox6->setDefaultText("N");
		gui.add(editBox6);

		auto button2 = tgui::Button::create();
		button2->setRenderer(theme.getRenderer("Button"));
		button2->setPosition(1080, 120);
		button2->setText("*");
		button2->setTextSize(20);
		button2->setSize(100, 50);
		gui.add(button2);
		button2->onPress([=] {
			int first, second;
			if (L != nullptr) {
				try {
					first = stoi(editBox3->getText().toStdString());
					second = stoi(editBox6->getText().toStdString());
					if (min(first, second) >= L->number && max(first, second) <= endL->number) {
						need_add(poly_sum_adder(multiply_poly(first, second), endL));
						display_on_data(listBox);
					}
				}
				catch (exception e) {
					pass;
				}
			}
		});


		auto editBox4 = tgui::EditBox::create();
		editBox4->setRenderer(theme.getRenderer("EditBox"));
		editBox4->setSize(50, 50);
		editBox4->setTextSize(18);
		editBox4->setPosition(950, 220);
		editBox4->setDefaultText("N");
		gui.add(editBox4);

		auto editBox5 = tgui::EditBox::create();
		editBox5->setRenderer(theme.getRenderer("EditBox"));
		editBox5->setSize(50, 50);
		editBox5->setTextSize(18);
		editBox5->setPosition(1020, 220);
		editBox5->setDefaultText("K");
		gui.add(editBox5);

		auto button3 = tgui::Button::create();
		button3->setRenderer(theme.getRenderer("Button"));
		button3->setPosition(1080, 220);
		button3->setText("F'(x)");
		button3->setTextSize(20);
		button3->setSize(100, 50);
		gui.add(button3);
		button3->onPress([=] {
			int first, second;
			if (L != nullptr) {
				try {
					first = stoi(editBox4->getText().toStdString());
					second = stoi(editBox5->getText().toStdString());
					if (first >= L->number && first <= endL->number) {
						node* tmp = L;
						while (tmp->number != first) {
							tmp = tmp->next;
						}
						if (second > tmp->poly->degree) {
							poly_sum_adder(poly_parser("0"), endL);
							display_on_data(listBox);
						}
						else {
							need_add(derivate_poly(first, second));
							display_on_data(listBox);
						}
					}
				}
				catch (exception e) {
					pass;
				}
			}
		});


		auto editBox7 = tgui::EditBox::create();
		editBox7->setRenderer(theme.getRenderer("EditBox"));
		editBox7->setSize(50, 50);
		editBox7->setTextSize(18);
		editBox7->setPosition(950, 320);
		editBox7->setDefaultText("N");
		gui.add(editBox7);


		auto button4 = tgui::Button::create();
		button4->setRenderer(theme.getRenderer("Button"));
		button4->setPosition(1080, 320);
		button4->setText("DELETE");
		button4->setTextSize(20);
		button4->setSize(100, 50);
		gui.add(button4);
		button4->onPress([=] {
			int first;
			if (L != nullptr) {
				try {
					first = stoi(editBox7->getText().toStdString());
					if (first >= L->number && first <= endL->number) {
						poly_deleter(first);
						listBox->removeItemByIndex(first - 1);
						if (endL != nullptr) {
							for (int i = first - 1; i < endL->number; ++i) {
								string s = listBox->getItemByIndex(i).toStdString();
								string s2 = "";
								for (int j = s.find(')'); j < s.size(); ++j) {
									s2 += s[j];
								}
								string tmp = "";
								for (auto& i : s) {
									if (i != ')')
										tmp.push_back(i);
									else
										break;
								}
								s2 = to_string(stoi(tmp) - 1) + s2;
								listBox->changeItemByIndex(i, s2);
							}
						}
					}
				}
				catch (exception e) {
					pass;
				}
			}
		});
	}
	catch (const tgui::Exception& e)
	{
		std::cerr << "TGUI Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}
int main() {
	

	tgui::Gui gui(window);
	if (!runExample(gui))
		return EXIT_FAILURE;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			gui.handleEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
			
		}

		window.clear();
		gui.draw();
		window.display();
	}
	return EXIT_SUCCESS;

	return 0;
}
