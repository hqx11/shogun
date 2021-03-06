#include <shogun/lib/common.h>
#include <shogun/lib/SGString.h>
#include <shogun/lib/SGVector.h>
#include <shogun/io/File.h>
#include <shogun/io/SGIO.h>

namespace shogun
{

template <class T>
SGString<T>::SGString() : string(NULL), slen(0), do_free(false) { }

template <class T>
SGString<T>::SGString(T* s, index_t l, bool free_s)
	: string(s), slen(l), do_free(free_s) { }

template <class T>
SGString<T>::SGString(SGVector<T> v)
	: string(v.vector), slen(v.vlen), do_free(false) { }

template <class T>
SGString<T>::SGString(index_t len, bool free_s) :
	slen(len), do_free(free_s)
{
	string=SG_CALLOC(T, len);
}

template <class T>
SGString<T>::SGString(const SGString &orig)
	: string(orig.string), slen(orig.slen), do_free(orig.do_free) { }

template <class T>
bool SGString<T>::operator==(const SGString & other) const
{
	if (other.slen != slen)
		return false;

	if (string != other.string)
		return false;

	return true;
}

template <class T>
bool SGString<T>::equals(const SGString& other) const
{
	// avoid comparing elements when both are same.
	// the case where both matrices are uninitialized is handled here as well.
	if (*this == other)
		return true;

	// both empty
	if (!(slen || other.slen))
		return true;

	// only one empty
	if (!string || !other.string)
		return false;

	// different size
	if (slen != other.slen)
		return false;

	// content
	return std::equal(string, string + slen, other.string);

	return true;
}

template<class T>
SGString<T> SGString<T>::clone() const
{
	SGString<T> result(slen);
	sg_memcpy(result.string, string, sizeof(T)*slen);
	return result;
}

template <class T>
void SGString<T>::free_string()
{
	if (do_free)
		SG_FREE(string);

	string=NULL;
	do_free=false;
	slen=0;
}

template <class T>
void SGString<T>::destroy_string()
{
	do_free=true;
	free_string();
}

template<class T> void SGString<T>::load(File* loader)
{
	ASSERT(loader)
	free_string();

	SG_SET_LOCALE_C;
	loader->get_vector(string, slen);
	do_free=true;
	SG_RESET_LOCALE;
}

template<class T> void SGString<T>::save(File* saver)
{
	ASSERT(saver)

	SG_SET_LOCALE_C;
	saver->set_vector(string, slen);
	SG_RESET_LOCALE;
}

template class SGString<bool>;
template class SGString<char>;
template class SGString<int8_t>;
template class SGString<uint8_t>;
template class SGString<int16_t>;
template class SGString<uint16_t>;
template class SGString<int32_t>;
template class SGString<uint32_t>;
template class SGString<int64_t>;
template class SGString<uint64_t>;
template class SGString<float32_t>;
template class SGString<float64_t>;
template class SGString<floatmax_t>;
}
