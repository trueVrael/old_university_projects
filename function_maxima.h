#ifndef FUNCTION_MAXIMA_H
#define FUNCTION_MAXIMA_H

#include <set>
#include <memory>

/* Zadanie 5, JNP
 * Autorzy : Tomasz Kaszlewicz, Daniel Le
 * Numery indeksow: tk334580, dl368567
 */

/* Klasa InvalidArg rzuca wyjatek kiedy argument podany funkcji value_at nie istnieje */

class InvalidArg : public std::exception {
	public:
		virtual const char* what() const throw(){
			return "InvalidArg";
		}
};

class if_null{
	public:
		void operator()(void const *) const noexcept {}
};

template<typename A, typename V>
class FunctionMaxima {
	public:
		class point_type{
			private:
			    std::shared_ptr<const A> arg_ptr;
			    std::shared_ptr<const V> val_ptr;
				point_type(std::shared_ptr<const A> arg, std::shared_ptr<const V> val) noexcept
				: arg_ptr(arg), val_ptr(val) {};
			public:
				point_type& operator=(const point_type &akt){
					*arg_ptr = *(akt.arg_ptr);
					*val_ptr = *(akt.val_ptr);
					return *this;
				}  
				A const& arg() const noexcept {
					return *arg_ptr;
				}
				V const& value() const noexcept {
					return *val_ptr;
				}
				friend class FunctionMaxima<A,V>;
		};
		/*	iterator i mx_iterator to nic innego jak iteratory wyciagniete z STLowego seta
		 *	nasza implementacja maximum i funkcji korzysta z setow
		 * 	w zwiazku z tym wiekszosc funkcji jest dziedziczona wprost z STLa
		 */
		using iterator = typename std::set<point_type>::iterator;
		using mx_iterator = typename std::set<point_type>::iterator;
		using size_type = typename std::set<point_type>::size_type;

		//rozmiar dziedziny
		size_type size() const noexcept{
			return f.size();
		}
		iterator begin() const noexcept{
			return f.begin();
		}
		iterator end() const noexcept{
			return f.end();
		}
		iterator find(const A &pt) const{
			if(f.size() == 0)
						return end();
			else{
				 std::shared_ptr<const A> pkt(&pt,if_null());
				 return f.find({pkt,begin()->val_ptr});
			}
		}
		mx_iterator mx_begin() const noexcept{
			return max.begin();
		}
		mx_iterator mx_end() const noexcept{
			return max.end();
		}          
		/*	funkcja set_value tworzy punkt o argumencie "a" i wartoĚ�?ci "v"
		 * 	kasuje dotychczasowy punkt o argumencie "a" jeĚ�?li istnieje i
		 * 	wkĚ�?ada nowy do seta
		 */
  		void set_value(const A &a, const V &v) {
  			 bool change = true;
        	 auto it = find(a);
        	 if(it != end()) {
             	if ((v < it->value()) || (it->value() < v)) {
                   point_type pkt = {it->arg_ptr, std::shared_ptr <const V>(new V(v))};
                   if (max.find(*it) != max.end()) max.erase(*it);
                   f.erase(*it);
                   f.insert(pkt);
         	 	}
         	 	else change = false;
		     } 
			 else{
    		 	  point_type p = {std::shared_ptr <const A>(new A(a)), std::shared_ptr <const V>(new V(v))};
            	  f.insert(p);
		     }
        // Naprawa maxim
   		     if(change){
             			it = find(a);
        	 			update(it);
        	 			if(it != end()){
             				  auto next = it;
            				  update(++next);
 				        }
        	 			if(it != begin()) update(--it);
   	 		}
 		 }
		/*	funkcja value_at zwraca wartosc funkcji w punkcie "a"
		 * 	wykorzystujac operacje find z seta 
		 */
		V const& value_at(A const& a) const{
			auto it = find(a);
			if (it == end()) throw InvalidArg(); //rzuc wyjatek
			else return it->value();
		}
		/*	funkcja erase usuwa z funkcji punkt o argumencie "a"
		 * 	wywoluje takze metode update, ktora aktualizuje status
		 * 	sasiadujacych punktow funkcji, ktore mogly stac sie maksimami
		 */
		void erase(A const& a){
			auto it = find(a);
			if (it != end()){
				if(max.find(*it) != max.end()) max.erase(max.find(*it));
			    auto next = it;
			    ++next;
			    if(next != end()) next = f.find(*next);
			    f.erase(it);
			    update(next);
			    if(next != begin()) update(--next);	
			}	
		}
	private:
        /*	funkcja update przyjmuje argument w postaci punktu i sprawdza
		 * 	czy podany punkt kwalifikuje sie do bycia maximum.
		 * 	sprawdza rownieĚź status "sasiadow" tego punktu, poniewaz mogl on sie zmienic
		 * 	zlozonosc O(log n) zapewnia set i jego drzewiasta struktura
		 */
		void update(const typename std::set<point_type>::iterator &it){
			 if(it != end()){
			 	bool max_l = false;
			 	bool max_r = false;
			 	auto prev = it;
			 	auto next = it;
			 	--prev;
			 	max_l = (it == begin() || !(it->value() < prev->value()));
			 	++next;
			 	max_r = (next ==end() || !(it->value() < next->value()));
			 	if(max_r && max_l) max.insert(*it);
			 	else max.erase(*it);
			 }
		}
		/*	podstawowy komparator sortujacy wzgledem argumentow funkcji
		 */
		struct comp{
			bool operator()(const point_type &a, const point_type &b) const{
				return a.arg() < b.arg();
			}
		};
		/*	komparator do maksimow funkcji
		 */
		struct maxima_comp{
			bool operator()(const point_type &a, const point_type &b) const{
				return b.value() < a.value() || ((!(a.value() < b.value())&&!(b.value() < a.value())) && a.arg() < b.arg());
			}
		};
		typedef std::set<point_type, comp> Funkcja;
		Funkcja f;
		typedef std::set<point_type, maxima_comp> Maxima;
		Maxima max;
};

#endif
