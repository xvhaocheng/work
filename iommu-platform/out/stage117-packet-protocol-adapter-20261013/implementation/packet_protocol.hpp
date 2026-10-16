#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

// Logical, single-owner protocol. No CPU/cache or application latency model.
namespace packet_protocol {
inline constexpr const char* cache_backend = "MODEL_UNCACHED_ASSUMPTION";
enum class State { dma_owned, allspans_complete, acquired, prefix_published,
                   consumer_completed, B_returned };
struct Range { uint64_t offset,length; };
struct Spec { unsigned packet, cookie, length; std::vector<unsigned> spans; std::vector<Range> destination_line_closure; };
struct Receipt { unsigned engine, span, packet, cookie; uint64_t id, generation; };
struct Token { unsigned packet=0, cookie=0; uint64_t generation=0, nonce=0; };
struct Packet {
    Spec spec; State state=State::dma_owned; uint64_t generation=1,nonce=0;
    bool source_retired=false, source_reused=false, validated=false;
};
class Protocol {
    struct Binding { bool reserved=false,accepted=false,done=false; Receipt receipt{}; };
    std::vector<Packet> packets_;
    std::vector<Binding> bindings_;
    std::array<uint64_t,2> last_next_{{1,1}};
    std::array<int,2> active_{{-1,-1}};
    size_t prefix_=0; uint64_t next_nonce_=0;
    bool quarantined_=false, mapped_=true;
    std::string reason_;
    static void require(bool ok,const char* msg){if(!ok)throw std::runtime_error(msg);}
    void live() const {require(!quarantined_,"allocation quarantined");require(mapped_,"mapping already released");}
    Packet& get(unsigned id){for(auto& p:packets_)if(p.spec.packet==id)return p;throw std::runtime_error("unknown packet");}
    void token_ok(const Packet& p,const Token* t)const {
        require(t && t->packet==p.spec.packet && t->cookie==p.spec.cookie &&
                t->generation==p.generation && t->nonce && t->nonce==p.nonce,
                "missing or stale cache acquire token");
    }
public:
    explicit Protocol(std::vector<Spec> specs,size_t spans):bindings_(spans){
        std::vector<bool> seen(spans,false);
        for(auto& s:specs){
            require(s.packet!=0 && !s.spans.empty(),"zero/empty packet");
            for(const auto& old:packets_)require(old.spec.packet!=s.packet && old.spec.cookie!=s.cookie,"duplicate packet/cookie");
            require(!s.destination_line_closure.empty(),"missing immutable line closure");
            for(size_t k=0;k<s.destination_line_closure.size();++k){
                const auto& a=s.destination_line_closure[k];
                require(a.length && !(a.offset%16) && !(a.length%16) && a.offset<16384 && a.length<=16384-a.offset,"invalid line closure");
                for(size_t j=0;j<k;++j){const auto& b=s.destination_line_closure[j];require(a.offset+a.length<=b.offset || b.offset+b.length<=a.offset,"self-overlapping closure");}
                for(const auto& old:packets_)for(const auto& b:old.spec.destination_line_closure)require(a.offset+a.length<=b.offset || b.offset+b.length<=a.offset,"cross-packet line ownership overlap");
            }
            for(unsigned i:s.spans){require(i<spans&&!seen[i],"span ownership");seen[i]=true;}
            packets_.push_back({s});
        }
        for(bool b:seen)require(b,"unowned span");
    }
    const Packet& packet(unsigned id) const {for(auto& p:packets_)if(p.spec.packet==id)return p;throw std::runtime_error("unknown packet");}
    bool quarantined()const{return quarantined_;}
    bool mapping_retained()const{return mapped_;}
    const std::string& reason()const{return reason_;}
    void uncertain(const std::string& why){quarantined_=true;reason_=why;}
    void timeout(){uncertain("public DONE/NEXT timeout");}
    void reserve(unsigned span,unsigned engine,unsigned pid,uint64_t generation,unsigned cookie){
        live();auto& p=get(pid);require(engine<2 && span<bindings_.size(),"reserve bounds");
        require(p.state==State::dma_owned && p.generation==generation && p.spec.cookie==cookie,"reserve packet identity/state");
        bool belongs=false;for(auto s:p.spec.spans)belongs|=s==span;require(belongs,"wrong packet span");
        auto& b=bindings_[span];require(!b.reserved && active_[engine]<0,"span/engine already reserved");
        b.reserved=true;b.receipt={engine,span,pid,cookie,0,generation};active_[engine]=int(span);
    }
    Receipt accept_next(unsigned engine,uint64_t id){
        live();require(engine<2 && active_[engine]>=0,"NEXT without reservation");
        auto& b=bindings_[active_[engine]];
        if(b.accepted || id==0 || id!=last_next_[engine]+1){uncertain("unknown NEXT acceptance");throw std::runtime_error(reason_);}
        b.accepted=true;b.receipt.id=id;last_next_[engine]=id;return b.receipt;
    }
    // IDs are engine-local: the whole receipt, never scalar ID, is the key.
    bool complete(const Receipt& r){
        live();require(r.engine<2 && r.span<bindings_.size(),"completion bounds");
        auto& b=bindings_[r.span];const auto& x=b.receipt;
        if(!b.accepted || b.done || active_[r.engine]!=int(r.span) ||
           r.engine!=x.engine || r.span!=x.span || r.packet!=x.packet ||
           r.cookie!=x.cookie || r.id!=x.id || r.generation!=x.generation){
            uncertain("stale duplicate or mismatched completion");throw std::runtime_error(reason_);
        }
        auto& p=get(r.packet);require(p.state==State::dma_owned && p.generation==r.generation,"completion packet generation");
        b.done=true;active_[r.engine]=-1;
        for(auto s:p.spec.spans)if(!bindings_[s].done)return false;
        p.state=State::allspans_complete;p.source_retired=true;return true;
    }
    Token acquire(unsigned pid,uint64_t generation){
        live();auto& p=get(pid);require(p.generation==generation && p.state==State::allspans_complete,"acquire requires complete current packet");
        // The model directly accesses behavioral memory. This does not execute fence.i.
        p.nonce=++next_nonce_;p.state=State::acquired;return {pid,p.spec.cookie,generation,p.nonce};
    }
    void allow_read(unsigned pid,const Token* t,bool consumer,uint64_t offset,uint64_t length)const{
        live();const auto& p=packet(pid);token_ok(p,t);
        require(p.state==(consumer?State::prefix_published:State::acquired),"packet read ownership");
        bool contained=false;
        for(const auto& r:p.spec.destination_line_closure)
            if(offset>=r.offset && offset-r.offset<r.length && length && length<=r.length-(offset-r.offset))contained=true;
        require(contained,"read outside acquired packet line closure");
    }
    void validated(unsigned pid,const Token* t){live();auto& p=get(pid);token_ok(p,t);require(p.state==State::acquired,"validation ownership");p.validated=true;}
    unsigned ready_prefix()const {live();if(prefix_==packets_.size())return 0;const auto& p=packets_[prefix_];return p.state==State::allspans_complete?p.spec.packet:0;}
    void publish(unsigned pid,const Token* t){
        live();auto& p=get(pid);token_ok(p,t);
        require(prefix_<packets_.size() && packets_[prefix_].spec.packet==pid,"out-of-order prefix");
        require(p.state==State::acquired && p.validated,"publish needs acquired validated packet");
        p.state=State::prefix_published;++prefix_;
    }
    void consumer_done(unsigned pid,const Token* t){live();auto& p=get(pid);token_ok(p,t);require(p.state==State::prefix_published,"consumer state");p.state=State::consumer_completed;}
    void return_B(unsigned pid){live();auto& p=get(pid);require(p.state==State::consumer_completed,"return before consumer");p.state=State::B_returned;}
    void allow_source_reuse(unsigned pid)const{live();const auto& p=packet(pid);require(p.source_retired && p.validated && !p.source_reused,"source reuse before retirement/validation or duplicate");}
    void source_reused(unsigned pid){allow_source_reuse(pid);get(pid).source_reused=true;}
    void teardown(bool checked_quiet){
        live();require(checked_quiet,"unchecked teardown");
        for(const auto& p:packets_)require(p.state==State::B_returned && p.source_reused,"teardown with owned packet");
        for(auto a:active_)require(a<0,"teardown active issuer");
        mapped_=false;
    }
    // Unit-only lawful reuse scenario; integration uses exactly one generation.
    void reuse_generation(){
        live();for(const auto& p:packets_)require(p.state==State::B_returned && p.source_reused,"generation reused early");
        for(auto a:active_)require(a<0,"generation active issuer");
        for(auto& p:packets_){++p.generation;p.state=State::dma_owned;p.nonce=0;p.source_retired=p.source_reused=p.validated=false;}
        for(auto& b:bindings_)b=Binding{};
        prefix_=0;
    }
};
}
