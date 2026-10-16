module dma_research_cost_groups #(
    parameter int unsigned StatWidth = 64
) (
    input  logic                 clk_i,
    input  logic                 rst_ni,
    input  logic                 clear_i,
    input  logic                 commit_i,
    input  logic [31:0]          main_i,
    input  logic [31:0]          comp_i,
    input  logic [31:0]          dur_i,
    input  logic [31:0]          req_i,
    input  logic [31:0]          miss_i,

    output logic [StatWidth-1:0] count_o    [6],
    output logic [StatWidth-1:0] sum_main_o [6],
    output logic [StatWidth-1:0] sum_comp_o [6],
    output logic [StatWidth-1:0] sum_dur_o  [6],
    output logic [StatWidth-1:0] sum_req_o  [6],
    output logic [StatWidth-1:0] sum_miss_o [6],
    output logic [31:0]          min_main_o [6],
    output logic [31:0]          max_main_o [6],
    output logic [31:0]          min_comp_o [6],
    output logic [31:0]          max_comp_o [6],
    output logic [31:0]          min_dur_o  [6],
    output logic [31:0]          max_dur_o  [6],
    output logic [StatWidth-1:0] total_o,
    output logic [15:0]          err_o
);
    localparam int unsigned NumGroups = 6;
    localparam int unsigned ExtWidth  = 65;

    logic [StatWidth-1:0] count_q    [NumGroups];
    logic [StatWidth-1:0] sum_main_q [NumGroups];
    logic [StatWidth-1:0] sum_comp_q [NumGroups];
    logic [StatWidth-1:0] sum_dur_q  [NumGroups];
    logic [StatWidth-1:0] sum_req_q  [NumGroups];
    logic [StatWidth-1:0] sum_miss_q [NumGroups];
    logic [31:0]          min_main_q [NumGroups];
    logic [31:0]          max_main_q [NumGroups];
    logic [31:0]          min_comp_q [NumGroups];
    logic [31:0]          max_comp_q [NumGroups];
    logic [31:0]          min_dur_q  [NumGroups];
    logic [31:0]          max_dur_q  [NumGroups];
    logic [NumGroups-1:0] seen_q;
    logic [StatWidth-1:0] total_q;
    logic [15:0]          err_q;

    logic [StatWidth-1:0] count_n    [NumGroups];
    logic [StatWidth-1:0] sum_main_n [NumGroups];
    logic [StatWidth-1:0] sum_comp_n [NumGroups];
    logic [StatWidth-1:0] sum_dur_n  [NumGroups];
    logic [StatWidth-1:0] sum_req_n  [NumGroups];
    logic [StatWidth-1:0] sum_miss_n [NumGroups];
    logic [31:0]          min_main_n [NumGroups];
    logic [31:0]          max_main_n [NumGroups];
    logic [31:0]          min_comp_n [NumGroups];
    logic [31:0]          max_comp_n [NumGroups];
    logic [31:0]          min_dur_n  [NumGroups];
    logic [31:0]          max_dur_n  [NumGroups];
    logic [NumGroups-1:0] seen_n;
    logic [StatWidth-1:0] total_n;
    logic [15:0]          err_n;

    logic [2:0]  grp;
    logic [64:0] ext;
    logic [32:0] main_plus_comp;

    always_comb begin
        for (int i = 0; i < NumGroups; i++) begin
            count_n[i]    = count_q[i];
            sum_main_n[i] = sum_main_q[i];
            sum_comp_n[i] = sum_comp_q[i];
            sum_dur_n[i]  = sum_dur_q[i];
            sum_req_n[i]  = sum_req_q[i];
            sum_miss_n[i] = sum_miss_q[i];
            min_main_n[i] = min_main_q[i];
            max_main_n[i] = max_main_q[i];
            min_comp_n[i] = min_comp_q[i];
            max_comp_n[i] = max_comp_q[i];
            min_dur_n[i]  = min_dur_q[i];
            max_dur_n[i]  = max_dur_q[i];
        end
        seen_n  = seen_q;
        total_n = total_q;
        err_n   = err_q;
        grp = 3'd5;
        ext = 65'd0;
        main_plus_comp = 33'd0;

        if ((req_i == 32'd4) && (miss_i <= 32'd4)) grp = miss_i[2:0];

        if (clear_i) begin
            for (int i = 0; i < NumGroups; i++) begin
                count_n[i]    = '0;
                sum_main_n[i] = '0;
                sum_comp_n[i] = '0;
                sum_dur_n[i]  = '0;
                sum_req_n[i]  = '0;
                sum_miss_n[i] = '0;
                min_main_n[i] = 32'd0;
                max_main_n[i] = 32'd0;
                min_comp_n[i] = 32'd0;
                max_comp_n[i] = 32'd0;
                min_dur_n[i]  = 32'd0;
                max_dur_n[i]  = 32'd0;
            end
            seen_n  = '0;
            total_n = '0;
            err_n   = 16'd0;
        end else if (commit_i) begin
            if (miss_i > req_i) err_n[2] = 1'b1;
            main_plus_comp = {1'b0, main_i} + {1'b0, comp_i};
            if (main_plus_comp > {1'b0, dur_i}) err_n[3] = 1'b1;

            if (&total_q) err_n[0] = 1'b1;
            total_n = total_q + 1'b1;

            if (&count_q[grp]) err_n[0] = 1'b1;
            count_n[grp] = count_q[grp] + 1'b1;

            ext = { { (ExtWidth-StatWidth) {1'b0} }, sum_main_q[grp] } +
                  { { (ExtWidth-32) {1'b0} }, main_i };
            if (|ext[ExtWidth-1:StatWidth]) err_n[1] = 1'b1;
            sum_main_n[grp] = ext[StatWidth-1:0];

            ext = { { (ExtWidth-StatWidth) {1'b0} }, sum_comp_q[grp] } +
                  { { (ExtWidth-32) {1'b0} }, comp_i };
            if (|ext[ExtWidth-1:StatWidth]) err_n[1] = 1'b1;
            sum_comp_n[grp] = ext[StatWidth-1:0];

            ext = { { (ExtWidth-StatWidth) {1'b0} }, sum_dur_q[grp] } +
                  { { (ExtWidth-32) {1'b0} }, dur_i };
            if (|ext[ExtWidth-1:StatWidth]) err_n[1] = 1'b1;
            sum_dur_n[grp] = ext[StatWidth-1:0];

            ext = { { (ExtWidth-StatWidth) {1'b0} }, sum_req_q[grp] } +
                  { { (ExtWidth-32) {1'b0} }, req_i };
            if (|ext[ExtWidth-1:StatWidth]) err_n[1] = 1'b1;
            sum_req_n[grp] = ext[StatWidth-1:0];

            ext = { { (ExtWidth-StatWidth) {1'b0} }, sum_miss_q[grp] } +
                  { { (ExtWidth-32) {1'b0} }, miss_i };
            if (|ext[ExtWidth-1:StatWidth]) err_n[1] = 1'b1;
            sum_miss_n[grp] = ext[StatWidth-1:0];

            if (!seen_q[grp]) begin
                min_main_n[grp] = main_i;
                max_main_n[grp] = main_i;
                min_comp_n[grp] = comp_i;
                max_comp_n[grp] = comp_i;
                min_dur_n[grp]  = dur_i;
                max_dur_n[grp]  = dur_i;
            end else begin
                if (main_i < min_main_q[grp]) min_main_n[grp] = main_i;
                if (main_i > max_main_q[grp]) max_main_n[grp] = main_i;
                if (comp_i < min_comp_q[grp]) min_comp_n[grp] = comp_i;
                if (comp_i > max_comp_q[grp]) max_comp_n[grp] = comp_i;
                if (dur_i  < min_dur_q[grp])  min_dur_n[grp]  = dur_i;
                if (dur_i  > max_dur_q[grp])  max_dur_n[grp]  = dur_i;
            end
            seen_n[grp] = 1'b1;
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            for (int i = 0; i < NumGroups; i++) begin
                count_q[i]    <= '0;
                sum_main_q[i] <= '0;
                sum_comp_q[i] <= '0;
                sum_dur_q[i]  <= '0;
                sum_req_q[i]  <= '0;
                sum_miss_q[i] <= '0;
                min_main_q[i] <= 32'd0;
                max_main_q[i] <= 32'd0;
                min_comp_q[i] <= 32'd0;
                max_comp_q[i] <= 32'd0;
                min_dur_q[i]  <= 32'd0;
                max_dur_q[i]  <= 32'd0;
            end
            seen_q  <= '0;
            total_q <= '0;
            err_q   <= 16'd0;
        end else begin
            for (int i = 0; i < NumGroups; i++) begin
                count_q[i]    <= count_n[i];
                sum_main_q[i] <= sum_main_n[i];
                sum_comp_q[i] <= sum_comp_n[i];
                sum_dur_q[i]  <= sum_dur_n[i];
                sum_req_q[i]  <= sum_req_n[i];
                sum_miss_q[i] <= sum_miss_n[i];
                min_main_q[i] <= min_main_n[i];
                max_main_q[i] <= max_main_n[i];
                min_comp_q[i] <= min_comp_n[i];
                max_comp_q[i] <= max_comp_n[i];
                min_dur_q[i]  <= min_dur_n[i];
                max_dur_q[i]  <= max_dur_n[i];
            end
            seen_q  <= seen_n;
            total_q <= total_n;
            err_q   <= err_n;
        end
    end

    always_comb begin
        for (int i = 0; i < NumGroups; i++) begin
            count_o[i]    = count_q[i];
            sum_main_o[i] = sum_main_q[i];
            sum_comp_o[i] = sum_comp_q[i];
            sum_dur_o[i]  = sum_dur_q[i];
            sum_req_o[i]  = sum_req_q[i];
            sum_miss_o[i] = sum_miss_q[i];
            min_main_o[i] = min_main_q[i];
            max_main_o[i] = max_main_q[i];
            min_comp_o[i] = min_comp_q[i];
            max_comp_o[i] = max_comp_q[i];
            min_dur_o[i]  = min_dur_q[i];
            max_dur_o[i]  = max_dur_q[i];
        end
        total_o = total_q;
        err_o   = err_q;
    end

endmodule
