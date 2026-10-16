// SPDX-License-Identifier: Solderpad-0.51
// COMP-only AW/WLAST/B association. AW and W order have independent cursors:
// W may precede AW. B uses per-ID AW order. B before WLAST is invalid AXI.
module dma_e3_write_observer (
    input logic clk_i,rst_ni,clear_i,run_i,active_i,open_i,close_i,
    input logic [31:0] time_i,
    input logic aw_i,wlast_i,b_i,
    input logic [3:0] aw_id_i,b_id_i,
    output logic [63:0] data_o[32],
    output logic error_o
);
    logic [2:0] present_q[8],present_n[8], inside_q[8],inside_n[8];
    logic role_q[8],role_n[8];
    logic [31:0] stamps_q[8][3],stamps_n[8][3];
    logic [2:0] aw_tail_q,aw_tail_n,w_tail_q,w_tail_n;
    logic [2:0] order_q[2][4],order_n[2][4];
    logic [1:0] head_q[2],head_n[2],tail_q[2],tail_n[2];
    logic [2:0] count_q[2],count_n[2];
    logic [63:0] data_q[32],data_n[32];
    logic bad;
    integer role,slot,base;
    logic [63:0] aw_w,w_b,aw_b;
    always_comb begin
        bad=error_o;aw_tail_n=aw_tail_q;w_tail_n=w_tail_q;
        role=0;slot=0;base=0;aw_w=0;w_b=0;aw_b=0;
        for(int j=0;j<8;j++) begin
            present_n[j]=present_q[j];inside_n[j]=inside_q[j];role_n[j]=role_q[j];
            for(int k=0;k<3;k++)stamps_n[j][k]=stamps_q[j][k];
        end
        for(int r=0;r<2;r++)begin
            head_n[r]=head_q[r];tail_n[r]=tail_q[r];count_n[r]=count_q[r];
            for(int j=0;j<4;j++)order_n[r][j]=order_q[r][j];
        end
        for(int j=0;j<32;j++)begin data_n[j]=data_q[j];data_o[j]=data_q[j];end
        if(aw_i)begin
            role=aw_id_i==4;slot=int'(aw_tail_q);
            if((aw_id_i!=0 && aw_id_i!=4) || present_n[slot][0] || count_q[role]==4)bad=1;
            present_n[slot][0]=1;inside_n[slot][0]=active_i;stamps_n[slot][0]=time_i;role_n[slot]=1'(role);
            order_n[role][tail_q[role]]=aw_tail_q;tail_n[role]=tail_q[role]+1'b1;
            count_n[role]=count_n[role]+1'b1;aw_tail_n=aw_tail_q+1'b1;
            if(active_i)data_n[role*16]=data_q[role*16]+1;
        end
        if(wlast_i)begin
            slot=int'(w_tail_q);
            if(present_n[slot][1])bad=1;
            present_n[slot][1]=1;inside_n[slot][1]=active_i;stamps_n[slot][1]=time_i;
            w_tail_n=w_tail_q+1'b1;
        end
        if(b_i)begin
            role=b_id_i==4;slot=int'(order_n[role][head_q[role]]);
            if((b_id_i!=0 && b_id_i!=4) || count_n[role]==0 ||
                !present_n[slot][0] || !present_n[slot][1] || present_n[slot][2])bad=1;
            present_n[slot][2]=1;inside_n[slot][2]=active_i;stamps_n[slot][2]=time_i;
            head_n[role]=head_q[role]+1'b1;count_n[role]=count_n[role]-1'b1;
            if(active_i)data_n[role*16+1]=data_q[role*16+1]+1;
        end
        for(int j=0;j<8;j++)if(&present_n[j])begin
            base=role_n[j] ? 16 : 0;
            if(inside_n[j][2])begin
                if(&inside_n[j])begin
                    data_n[base+2]=data_n[base+2]+1;
                    if(stamps_n[j][1]<stamps_n[j][0])data_n[base+4]=data_n[base+4]+1;
                    else begin
                        aw_w=64'(stamps_n[j][1]-stamps_n[j][0]);
                        data_n[base+5]=data_n[base+5]+aw_w;
                        if(aw_w>data_n[base+6])data_n[base+6]=aw_w;
                    end
                    w_b=64'(stamps_n[j][2]-stamps_n[j][1]);
                    aw_b=64'(stamps_n[j][2]-stamps_n[j][0]);
                    data_n[base+7]=data_n[base+7]+w_b;
                    if(w_b>data_n[base+8])data_n[base+8]=w_b;
                    data_n[base+9]=data_n[base+9]+aw_b;
                    if(aw_b>data_n[base+10])data_n[base+10]=aw_b;
                end else data_n[base+3]=data_n[base+3]+1;
            end
            present_n[j]=0;inside_n[j]=0;
        end
        if(open_i)for(int r=0;r<2;r++)data_n[r*16+13]=64'(count_n[r]);
        if(close_i)begin
            for(int r=0;r<2;r++)data_n[r*16+11]=64'(count_n[r]);
            data_n[12]=0;
            for(int j=0;j<8;j++)if(present_n[j][1] && !present_n[j][0])data_n[12]=data_n[12]+1;
        end
        // All cumulative fields are monotonic; overflow must invalidate the run.
        for(int j=0;j<32;j++)if(j%16<11 && data_n[j]<data_q[j])bad=1;
    end
    always_ff @(posedge clk_i or negedge rst_ni)begin
        if(!rst_ni)begin
            error_o<=0;aw_tail_q<=0;w_tail_q<=0;
            for(int j=0;j<8;j++)begin
                present_q[j]<=0;inside_q[j]<=0;role_q[j]<=0;
                for(int k=0;k<3;k++)stamps_q[j][k]<=0;
            end
            for(int r=0;r<2;r++)begin
                head_q[r]<=0;tail_q[r]<=0;count_q[r]<=0;
                for(int j=0;j<4;j++)order_q[r][j]<=0;
            end
            for(int j=0;j<32;j++)data_q[j]<=0;
        end else if(clear_i)begin
            error_o<=0;aw_tail_q<=0;w_tail_q<=0;
            for(int j=0;j<8;j++)begin present_q[j]<=0;inside_q[j]<=0;end
            for(int r=0;r<2;r++)begin head_q[r]<=0;tail_q[r]<=0;count_q[r]<=0;end
            for(int j=0;j<32;j++)data_q[j]<=0;
        end else if(run_i)begin
            error_o<=bad;aw_tail_q<=aw_tail_n;w_tail_q<=w_tail_n;
            for(int j=0;j<8;j++)begin
                present_q[j]<=present_n[j];inside_q[j]<=inside_n[j];role_q[j]<=role_n[j];
                for(int k=0;k<3;k++)stamps_q[j][k]<=stamps_n[j][k];
            end
            for(int r=0;r<2;r++)begin
                head_q[r]<=head_n[r];tail_q[r]<=tail_n[r];count_q[r]<=count_n[r];
                for(int j=0;j<4;j++)order_q[r][j]<=order_n[r][j];
            end
            for(int j=0;j<32;j++)data_q[j]<=data_n[j];
        end
    end
endmodule
