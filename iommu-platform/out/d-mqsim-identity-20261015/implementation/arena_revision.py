from pathlib import Path
p=Path(__file__).parent/'source/src/identity_log.h';s=p.read_text()
s=s.replace('#include <iostream>','#include <iostream>\n#include <cstddef>\n#include <new>\n#include <utility>')
s=s.replace('namespace Identity {','''namespace Identity {
// Dedicated BSS arena: observer map nodes never consume the model malloc heap.
// No reclamation is needed for this single fresh-process frozen workload.
struct Arena {
 alignas(std::max_align_t) unsigned char bytes[32u*1024u*1024u];
 size_t used=0;
};
inline Arena& arena() { static Arena a; return a; }
template<class T> struct Allocator {
 typedef T value_type;
 template<class U> struct rebind { typedef Allocator<U> other; };
 Allocator() noexcept {}
 template<class U> Allocator(const Allocator<U>&) noexcept {}
 T* allocate(size_t n) {
  Arena& a=arena();const size_t alignment=alignof(T);
  const size_t start=(a.used+alignment-1)&~(alignment-1);
  if(start>sizeof(a.bytes) || n>(sizeof(a.bytes)-start)/sizeof(T)) throw std::bad_alloc();
  a.used=start+n*sizeof(T);return reinterpret_cast<T*>(a.bytes+start);
 }
 void deallocate(T*,size_t) noexcept {}
};
template<class T,class U> inline bool operator==(const Allocator<T>&,const Allocator<U>&) {return true;}
template<class T,class U> inline bool operator!=(const Allocator<T>&,const Allocator<U>&) {return false;}
template<class V> using Map=std::map<const void*,V,std::less<const void*>,Allocator<std::pair<const void* const,V>>>;
''')
s=s.replace('uint64_t host_next=0, child_next=0;','uint64_t host_next=0, child_next=0;\n bool disabled=false;')
s=s.replace('std::map<const void*,uint64_t>', 'Map<uint64_t>').replace('std::map<const void*,Child>', 'Map<Child>')
# Every public event hook is an allocation-failure boundary. Nested calls remain safe.
names=['host_generated','sqe_bind','user_bind','user_event','child_event','child_retire','child_live_event','child_created','cqe_send','host_completed']
for name in names:
 start=s.index('void '+name+'(');body=s.index('{',start);depth=1;end=body+1
 while depth:
  if s[end]=='{':depth+=1
  if s[end]=='}':depth-=1
  end+=1
 s=s[:body+1]+'\n if(state().disabled)return;\n try {'+s[body+1:end-1]+''' } catch(const std::bad_alloc&) {
  state().disabled=true;error(t,"observer_arena_exhausted_identity_disabled",nullptr);
 }
'''+s[end-1:]
p.write_text(s)
