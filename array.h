#ifndef ARRAY__H
#define ARRAY__H

#include <assert.h>
#include <sstream>
#include <string>

namespace storage
{
    /*
     * This class is model of boost::uBlas Storage concept.
     */
	template <typename T>
	class Array
	{
	public:
		typedef T value_type;
		//	temporary decision
		typedef int ptrdiff_t;
		//	associated
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef pointer iterator;
		typedef const_pointer const_iterator;

	public:
		//	default constructor
		Array(size_type size = 0) :m_data(0) {
			this->_init(size);
		}
		Array(const Array &a) {
			if (!a.m_size || !a.m_data) {
				//	empty
				m_data = 0;
				m_size = 0;
				return;
			}
			m_size = a.m_size;
			m_data = new T[m_size];
			for (size_type i = 0; i < m_size; i++) {
				*(m_data + i) = *(a.m_data + i);
			}
		}
		Array(size_type size, pointer p) :m_data(0) {
			this->_init(size, p);
		}
		virtual ~Array() {
			if (m_data)
				delete[]m_data;
		}

	public:
		iterator begin() {
			return m_data;
		}
		iterator end() {
			return m_data + m_size;
		}
		/*	element access	*/
		reference operator[](size_type i) {
			return this->at(i);
		}
		const_reference operator[](size_type i)const {
			if (!m_data)
				return m_default;
			return *(m_data + i);
		}
		/*	additional	*/
		reference at(size_type i){
			if (!m_data)
				/*	Here default value may be changed.	*/
				return m_default;
			return *(m_data + i);
		}
		const_reference const_at(size_type i)const {
			if (!m_data)
				/*	Here default value may be changed.	*/
				return m_default;
			return *(m_data + i);
		}
		/*	change array size	*/
		void resize(size_type size) {
			if (m_data)
				delete[]m_data;
			m_data = new T[size];
			m_size = size;
		}
		void resize(size_type size, const value_type& t) {
			if (m_data)
				delete[]m_data;
			m_data = new T[size];
			for (size_type i = 0; i < size; i++) {
				*(m_data + i) = t;
			}
			m_size = size;
		}
		/*	additional	*/
		void resize(size_type size, pointer p) {
			this->_init(size, p);
		}
		size_type size()const {
			return m_size;
		}
		size_type max_size()const {
			return 0;
		}
		Array & operator=(const Array &a) {
			this->_init(a.m_size, a.m_data);
			return *this;
		}
		void swap(const Array &a) {
			/*	not realized yet	*/
		}
		/*	additional	*/
		bool empty()const {
			return m_data != 0 && 0 < m_size;
		}
		/*	delete all data from the array	*/
		void release() {
			if (m_data) {
				delete[]m_data;
			}
			m_size = 0;
		}
		/*	
		* Assign default values to all elements.
		* May be overriden in derived classes.	
		*/
		virtual void clear(){
			if (this->empty())
				return;
			for (size_type i = 0; i < m_size; i++) {
				*(m_data + i) = value_type();
			}
		}
		void setDefault(value_type default_) {
			m_default = default_;
		}
        void getRaw(pointer buf,size_type buf_len)const{
            if(buf_len<m_size)
                return;
            memcpy(buf,m_data,m_size*sizeof(value_type));
        }
	protected:
		/*	Internal methods which allows to derivative classes get access to raw data.	*/
		pointer _raw() {
			return m_data;
		}
		const_pointer _raw()const {
			return m_data;
		}
		/*	Internal method which SHOULD NOT be called explicitly	*/
		void _init(size_type size, pointer p = 0) {
			if (!size) {
				/*	equivalent to call of 'release'	*/
				if (m_data)
					delete[]m_data;
				m_data = 0;
				m_size = 0;
				return;
			}
			if (m_size != size) {
				/*	the size is not zero	*/
				if(m_data)
					delete[]m_data;
                m_data = new value_type[size];
				m_size = size;
			}
			if (p) {
				/*	copy memory region	*/
                memcpy(m_data, p, sizeof(value_type)*m_size);
			}
			else {
				for (size_type i = 0; i < size; i++) {
					/*	DefaultConstructible concept	*/
                    *(m_data + i) = value_type();
				}
			}
		}
	public:
		/*	Convert object to string representation	*/
		virtual std::string toString() const{
			std::stringbuf buf;
			std::ostream os(&buf);
			os << "[";
			size_type size = this->size();
			if (0 < size) {
				//	add first element without preceeding comma
				os << this->operator[](0);
			}
			else {
				os << "]";
				return buf.str();
			}
			for (size_type i = 1; i < size; i++) {
				os << "," << this->operator[](i);
			}
			os << "]";
			return buf.str();
		}
		virtual std::string toJSON() const {
			return "";
		}
	protected:
		size_type m_size;
		/*	data	*/
		pointer m_data;
	protected:
		/*	used to return value in unexpected situations and in derived classes.	*/
		value_type m_default;
	};	/*	Array	*/
}
#endif//__SURFACE__ARRAY__H
