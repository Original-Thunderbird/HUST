
z
eCannot write hardware definition file as there are no IPI block design hardware handoff files present*	vivadotclh px
N
Command: %s
53*	vivadotcl2 
place_design2default:defaultZ4-113h px
�
@Attempting to get a license for feature '%s' and/or device '%s'
308*common2"
Implementation2default:default2
xc7a100t2default:defaultZ17-347h px
�
0Got license for feature '%s' and/or device '%s'
310*common2"
Implementation2default:default2
xc7a100t2default:defaultZ17-349h px
M
Running DRC with %s threads
24*drc2
42default:defaultZ23-27h px
S
DRC finished with %s
79*	vivadotcl2
0 Errors2default:defaultZ4-198h px
b
BPlease refer to the DRC report (report_drc) for more information.
80*	vivadotclZ4-199h px
m
,Running DRC as a precondition to command %s
22*	vivadotcl2 
place_design2default:defaultZ4-22h px
M
Running DRC with %s threads
24*drc2
42default:defaultZ23-27h px
S
DRC finished with %s
79*	vivadotcl2
0 Errors2default:defaultZ4-198h px
b
BPlease refer to the DRC report (report_drc) for more information.
80*	vivadotclZ4-199h px
R

Starting %s Task
103*constraints2
Placer2default:defaultZ18-103h px
z
BMultithreading enabled for place_design using a maximum of %s CPUs12*	placeflow2
42default:defaultZ30-611h px
v

Phase %s%s
101*constraints2
1 2default:default2,
Placer Runtime Estimator2default:defaultZ18-101h px
H
6Phase 1 Placer Runtime Estimator | Checksum: dcccef42
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:01 ; elapsed = 00:00:03 . Memory (MB): peak = 1609.039 ; gain = 0.000 ; free physical = 105 ; free virtual = 9762default:defaulth px
s

Phase %s%s
101*constraints2
2 2default:default2)
Placer Initialization2default:defaultZ18-101h px
�
r%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s ; free physical = %s ; free virtual = %s
480*common2.
Netlist sorting complete. 2default:default2
00:00:002default:default2
00:00:00.042default:default2
1609.0392default:default2
0.0002default:default2
1042default:default2
9752default:defaultZ17-722h px
�
r%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s ; free physical = %s ; free virtual = %s
480*common2.
Netlist sorting complete. 2default:default2
00:00:002default:default2
00:00:00.012default:default2
1609.0392default:default2
0.0002default:default2
1042default:default2
9752default:defaultZ17-722h px
�

Phase %s%s
101*constraints2
2.1 2default:default2F
2IO Placement/ Clock Placement/ Build Placer Device2default:defaultZ18-101h px
q

Phase %s%s
101*constraints2
2.1.1 2default:default2#
Pre-Place Cells2default:defaultZ18-101h px
C
1Phase 2.1.1 Pre-Place Cells | Checksum: 6a6e8315
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:02 ; elapsed = 00:00:04 . Memory (MB): peak = 1609.039 ; gain = 0.000 ; free physical = 103 ; free virtual = 9752default:defaulth px
B
%Done setting XDC timing constraints.
35*timingZ38-35h px
s

Phase %s%s
101*constraints2
2.1.2 2default:default2%
IO & Clk Clean Up2default:defaultZ18-101h px
�
9Poor placement for routing between an IO pin and BUFG. %s528*place2�
�This is normally an ERROR but the CLOCK_DEDICATED_ROUTE constraint is set to FALSE allowing your design to continue. The use of this override is highly discouraged as it may lead to very poor timing results. It is recommended that this error condition be corrected in the design.

	<MSGMETA::BEGIN::BLOCK>CP_IBUF_inst<MSGMETA::END> (IBUF.O) is locked to IOB_X0Y50
	<MSGMETA::BEGIN::BLOCK>CP_IBUF_BUFG_inst<MSGMETA::END> (BUFG.I) is provisionally placed by clockplacer on BUFGCTRL_X0Y0
"�
CP_IBUF_inst2�This is normally an ERROR but the CLOCK_DEDICATED_ROUTE constraint is set to FALSE allowing your design to continue. The use of this override is highly discouraged as it may lead to very poor timing results. It is recommended that this error condition be corrected in the design.

	:" (IBUF.O) is locked to IOB_X0Y50
	"Y
CP_IBUF_BUFG_inst:B (BUFG.I) is provisionally placed by clockplacer on BUFGCTRL_X0Y0
2default:default8Z30-574h px
E
3Phase 2.1.2 IO & Clk Clean Up | Checksum: 6a6e8315
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:20 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 102 ; free virtual = 9682default:defaulth px
�

Phase %s%s
101*constraints2
2.1.3 2default:default2>
*Implementation Feasibility check On IDelay2default:defaultZ18-101h px
^
LPhase 2.1.3 Implementation Feasibility check On IDelay | Checksum: 6a6e8315
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:20 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 102 ; free virtual = 9682default:defaulth px
u

Phase %s%s
101*constraints2
2.1.4 2default:default2'
Commit IO Placement2default:defaultZ18-101h px
G
5Phase 2.1.4 Commit IO Placement | Checksum: 41372d75
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:20 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 102 ; free virtual = 9682default:defaulth px
e
SPhase 2.1 IO Placement/ Clock Placement/ Build Placer Device | Checksum: 117b95c11
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:20 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 102 ; free virtual = 9682default:defaulth px
z

Phase %s%s
101*constraints2
2.2 2default:default2.
Build Placer Netlist Model2default:defaultZ18-101h px
s

Phase %s%s
101*constraints2
2.2.1 2default:default2%
Place Init Design2default:defaultZ18-101h px
F
4Phase 2.2.1 Place Init Design | Checksum: 1f4698fc0
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:21 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 98 ; free virtual = 9682default:defaulth px
M
;Phase 2.2 Build Placer Netlist Model | Checksum: 1f4698fc0
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:21 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 98 ; free virtual = 9682default:defaulth px
w

Phase %s%s
101*constraints2
2.3 2default:default2+
Constrain Clocks/Macros2default:defaultZ18-101h px
�

Phase %s%s
101*constraints2
2.3.1 2default:default24
 Constrain Global/Regional Clocks2default:defaultZ18-101h px
U
CPhase 2.3.1 Constrain Global/Regional Clocks | Checksum: 1f4698fc0
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:21 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 98 ; free virtual = 9682default:defaulth px
J
8Phase 2.3 Constrain Clocks/Macros | Checksum: 1f4698fc0
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:21 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 98 ; free virtual = 9682default:defaulth px
F
4Phase 2 Placer Initialization | Checksum: 1f4698fc0
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:09 ; elapsed = 00:00:21 . Memory (MB): peak = 1645.035 ; gain = 35.996 ; free physical = 98 ; free virtual = 9682default:defaulth px
n

Phase %s%s
101*constraints2
3 2default:default2$
Global Placement2default:defaultZ18-101h px
A
/Phase 3 Global Placement | Checksum: 1322a17d6
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:18 ; elapsed = 00:00:32 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 103 ; free virtual = 9582default:defaulth px
n

Phase %s%s
101*constraints2
4 2default:default2$
Detail Placement2default:defaultZ18-101h px
z

Phase %s%s
101*constraints2
4.1 2default:default2.
Commit Multi Column Macros2default:defaultZ18-101h px
M
;Phase 4.1 Commit Multi Column Macros | Checksum: 1322a17d6
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:19 ; elapsed = 00:00:32 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 103 ; free virtual = 9582default:defaulth px
|

Phase %s%s
101*constraints2
4.2 2default:default20
Commit Most Macros & LUTRAMs2default:defaultZ18-101h px
O
=Phase 4.2 Commit Most Macros & LUTRAMs | Checksum: 16a14878a
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:19 ; elapsed = 00:00:32 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 103 ; free virtual = 9582default:defaulth px
v

Phase %s%s
101*constraints2
4.3 2default:default2*
Area Swap Optimization2default:defaultZ18-101h px
I
7Phase 4.3 Area Swap Optimization | Checksum: 1a87c6d13
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:19 ; elapsed = 00:00:33 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 103 ; free virtual = 9582default:defaulth px
|

Phase %s%s
101*constraints2
4.4 2default:default20
Small Shape Detail Placement2default:defaultZ18-101h px
�

Phase %s%s
101*constraints2
4.4.1 2default:default24
 Commit Small Macros & Core Logic2default:defaultZ18-101h px
y

Phase %s%s
101*constraints2
4.4.1.1 2default:default2)
Commit Slice Clusters2default:defaultZ18-101h px
L
:Phase 4.4.1.1 Commit Slice Clusters | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:22 ; elapsed = 00:00:38 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 104 ; free virtual = 9482default:defaulth px
U
CPhase 4.4.1 Commit Small Macros & Core Logic | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:22 ; elapsed = 00:00:38 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 104 ; free virtual = 9482default:defaulth px
�

Phase %s%s
101*constraints2
4.4.2 2default:default2C
/Clock Restriction Legalization for Leaf Columns2default:defaultZ18-101h px
d
RPhase 4.4.2 Clock Restriction Legalization for Leaf Columns | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:22 ; elapsed = 00:00:38 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 104 ; free virtual = 9482default:defaulth px
�

Phase %s%s
101*constraints2
4.4.3 2default:default2E
1Clock Restriction Legalization for Non-Clock Pins2default:defaultZ18-101h px
f
TPhase 4.4.3 Clock Restriction Legalization for Non-Clock Pins | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:22 ; elapsed = 00:00:38 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 104 ; free virtual = 9472default:defaulth px
O
=Phase 4.4 Small Shape Detail Placement | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:22 ; elapsed = 00:00:38 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9482default:defaulth px
r

Phase %s%s
101*constraints2
4.5 2default:default2&
Re-assign LUT pins2default:defaultZ18-101h px
E
3Phase 4.5 Re-assign LUT pins | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:22 ; elapsed = 00:00:38 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9482default:defaulth px
A
/Phase 4 Detail Placement | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:22 ; elapsed = 00:00:38 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9482default:defaulth px
�

Phase %s%s
101*constraints2
5 2default:default2<
(Post Placement Optimization and Clean-Up2default:defaultZ18-101h px
s

Phase %s%s
101*constraints2
5.1 2default:default2'
PCOPT Shape updates2default:defaultZ18-101h px
F
4Phase 5.1 PCOPT Shape updates | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:23 ; elapsed = 00:00:39 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9482default:defaulth px
v

Phase %s%s
101*constraints2
6 2default:default2,
Post Commit Optimization2default:defaultZ18-101h px
I
7Phase 6 Post Commit Optimization | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:23 ; elapsed = 00:00:39 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9482default:defaulth px
�

Phase %s%s
101*constraints2
5.2 2default:default25
!Sweep Clock Roots: Post-Placement2default:defaultZ18-101h px
T
BPhase 5.2 Sweep Clock Roots: Post-Placement | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:23 ; elapsed = 00:00:39 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9482default:defaulth px
v

Phase %s%s
101*constraints2
5.3 2default:default2*
Post Placement Cleanup2default:defaultZ18-101h px
I
7Phase 5.3 Post Placement Cleanup | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:23 ; elapsed = 00:00:39 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9482default:defaulth px
p

Phase %s%s
101*constraints2
5.4 2default:default2$
Placer Reporting2default:defaultZ18-101h px
C
1Phase 5.4 Placer Reporting | Checksum: 24ae98abb
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:23 ; elapsed = 00:00:39 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9482default:defaulth px
w

Phase %s%s
101*constraints2
5.5 2default:default2+
Final Placement Cleanup2default:defaultZ18-101h px
J
8Phase 5.5 Final Placement Cleanup | Checksum: 1dcb9554b
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:23 ; elapsed = 00:00:39 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9492default:defaulth px
Y
GPhase 5 Post Placement Optimization and Clean-Up | Checksum: 1dcb9554b
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:23 ; elapsed = 00:00:39 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9492default:defaulth px
;
)Ending Placer Task | Checksum: 10cf9ef76
*commonh px
�

%s
*constraints2�
�Time (s): cpu = 00:00:23 ; elapsed = 00:00:39 . Memory (MB): peak = 1661.043 ; gain = 52.004 ; free physical = 105 ; free virtual = 9492default:defaulth px
W
Releasing license: %s
83*common2"
Implementation2default:defaultZ17-83h px
�
G%s Infos, %s Warnings, %s Critical Warnings and %s Errors encountered.
28*	vivadotcl2
332default:default2
12default:default2
02default:default2
02default:defaultZ4-41h px
[
%s completed successfully
29*	vivadotcl2 
place_design2default:defaultZ4-42h px
�
r%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s ; free physical = %s ; free virtual = %s
480*common2"
place_design: 2default:default2
00:00:372default:default2
00:01:062default:default2
1661.0432default:default2
52.0082default:default2
1052default:default2
9492default:defaultZ17-722h px
A
Writing placer database...
1603*designutilsZ20-1893h px
:
Writing XDEF routing.
211*designutilsZ20-211h px
G
#Writing XDEF routing logical nets.
209*designutilsZ20-209h px
G
#Writing XDEF routing special nets.
210*designutilsZ20-210h px
�
r%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s ; free physical = %s ; free virtual = %s
480*common2)
Write XDEF Complete: 2default:default2
00:00:00.212default:default2
00:00:00.432default:default2
1661.0432default:default2
0.0002default:default2
1052default:default2
9502default:defaultZ17-722h px
�
�report_io: Time (s): cpu = 00:00:01 ; elapsed = 00:00:03 . Memory (MB): peak = 1661.043 ; gain = 0.000 ; free physical = 100 ; free virtual = 949
*commonh px
�
�report_utilization: Time (s): cpu = 00:00:00.75 ; elapsed = 00:00:02 . Memory (MB): peak = 1661.043 ; gain = 0.000 ; free physical = 99 ; free virtual = 949
*commonh px
�
�report_control_sets: Time (s): cpu = 00:00:00.01 ; elapsed = 00:00:00.04 . Memory (MB): peak = 1661.043 ; gain = 0.000 ; free physical = 99 ; free virtual = 949
*commonh px


End Record