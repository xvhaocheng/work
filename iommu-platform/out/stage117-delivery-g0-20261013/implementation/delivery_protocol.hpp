#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
// Controlled one-owner, nofault success projection, not a production driver API.
class DeliveryProtocol {
public:
 struct Packet {bool complete=false,source_retired=false,A_returned=false,source_metadata_retired=false,acquired=false,published=false,consumed=false,B_returned=false;uint64_t nonce=0;};
 std::array<Packet,3> packets{};std::array<bool,4> span_done{};unsigned published_prefix=0;uint64_t next_nonce=0;
 Packet& packet(unsigned pid){CHECK(pid>=1&&pid<=3,"packet identity");return packets[pid-1];}
 void retire_source(unsigned pid){auto& p=packet(pid);CHECK(!p.source_retired,"duplicate source retirement");p.source_retired=true;}
 void return_A(unsigned pid){auto& p=packet(pid);CHECK(p.source_retired&&!p.A_returned,"A return without source retirement");p.A_returned=true;}
 bool complete_dma(unsigned index){CHECK(index<4&&!span_done[index],"duplicate DMA completion");span_done[index]=true;unsigned pid=c117::spans().at(index).packet;for(unsigned k=0;k<4;++k)if(c117::spans()[k].packet==pid&&!span_done[k])return false;auto& p=packet(pid);CHECK(!p.complete,"duplicate packet completion");p.complete=true;retire_source(pid);return true;}
 void complete_cpu(unsigned pid){auto& p=packet(pid);CHECK(p.source_retired&&!p.complete,"CPU complete before read retirement");p.complete=true;}
 std::vector<unsigned> ready_prefix()const{std::vector<unsigned> ids;for(unsigned i=published_prefix;i<3&&packets[i].complete;++i)ids.push_back(i+1);return ids;}
 void acquire(unsigned pid,uint64_t receipt){auto& p=packet(pid);CHECK(p.complete&&!p.acquired&&receipt,"acquire before actual completed handoff");p.acquired=true;p.nonce=++next_nonce;}
 void publish(unsigned pid){auto& p=packet(pid);CHECK(pid==published_prefix+1&&p.acquired&&!p.published,"used not complete prefix");p.published=true;++published_prefix;}
 void allow_read(unsigned pid,uint64_t offset,uint64_t n){const auto& p=packet(pid);CHECK(p.published&&p.acquired&&!p.consumed,"consumer ownership");bool okay=false;for(const auto& d:c117::packets()[pid-1].destination){uint64_t lo=d.offset&~15ULL,hi=(d.offset+d.capacity+15)&~15ULL;if(offset>=lo&&offset<hi&&n&&n<=hi-offset)okay=true;}CHECK(okay,"consumer outside own line closure");}
 void consume(unsigned pid){auto& p=packet(pid);CHECK(p.published&&!p.consumed,"consumer state");p.consumed=true;}
 void return_B(unsigned pid){auto& p=packet(pid);CHECK(p.consumed&&!p.B_returned,"B return before actual consumption");p.B_returned=true;}
 void teardown(){for(const auto& p:packets)CHECK(p.B_returned&&p.A_returned&&p.source_metadata_retired,"release while owned");}
};
