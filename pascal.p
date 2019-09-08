program PASCALS(INPUT,OUTPUT,PRD,PRR);
{  author:N.Wirth, E.T.H. CH-8092 Zurich,1.3.76 }
{  modified by R.E.Berry
    Department of computer studies
    University of Lancaster

    Variants of this program are used on
    Data General Nova,Apple,and
    Western Digital Microengine machines. }
{   further modified by M.Z.Jin
    Department of Computer Science&Engineering BUAA,0ct.1989
}
const nkw = 27;    { no. of key words }{*关键字长度*}
      alng = 10;   { no. of significant chars in identifiers }{*标识符中的重要字符的长度*}
      llng = 121;  { input line length }{*输入行的长度*}
      emax = 322;  { max exponent of real numbers }{*实数的最大指数*}
      emin = -292; { min exponent }{*最小的指数*}
      kmax = 15;   { max no. of significant digits }{*有效数字的最大长度*}
      tmax = 100;  { size of table }{*表的大小*}
      bmax = 20;   { size of block-talbe }{*块表的大小*}
      amax = 30;   { size of array-table }{*数组表的大小*}
      c2max = 20;  { size of real constant table }{*实常数表的大小*}
      csmax = 30;  { max no. of cases }
      cmax = 800;  { size of code }{*代码大小*}
      lmax = 7;    { maximum level }{*最大深度*}
      smax = 600;  { size of string-table }{*字串表的大小*}
      ermax = 58;  { max error no. }{*错误数目的最大值*}
      omax = 63;   { highest order code }{*最高订单代码*}
      xmax = 32767;  { 2**15-1 }
      nmax = 32767;  { 2**15-1 }
      lineleng = 132; { output line length }{*输出行长度*}
      linelimit = 200;
      stacksize = 1450;{*堆栈大小*}
type symbol = ( intcon, realcon, charcon, stringcon,
                notsy, plus, minus, times, idiv, rdiv, imod, andsy, orsy,
                eql, neq, gtr, geq, lss, leq,
                lparent, rparent, lbrack, rbrack, comma, semicolon, period,
                colon, becomes, constsy, typesy, varsy, funcsy,
                procsy, arraysy, recordsy, programsy, ident,
                beginsy, ifsy, casesy, repeatsy, whilesy, forsy,
                endsy, elsesy, untilsy, ofsy, dosy, tosy, downtosy, thensy);
     index = -xmax..+xmax;{*index赋值为-32767到32767*}
     alfa = packed array[1..alng]of char;{*char alfa[1...10]*}
     objecttyp = (konstant, vvariable, typel, prozedure, funktion ); 
     types = (notyp, ints, reals, bools, chars, arrays, records );
     symset = set of symbol;{*包含symbol元素的集合*}
     typset = set of types;{*包含types元素的集合*}
     item = record
               typ: types;
               ref: index;
            end;

     order = packed record
                f: -omax..+omax;{*-63至63*}
                x: -lmax..+lmax;{*-7到7*}
                y: -nmax..+nmax{*-32767至32767*}
            end;
var   ch:         char; { last character read from source program }{*从源程序读取的最后一个字符*}
      rnum:       real; { real number from insymbol }{*来自insymbol的实数*}
      inum:       integer;     { integer from insymbol }{*来自insymbol的整数*}
      sleng:      integer;     { string length }{*字符串长度*}
      cc:         integer;     { character counter }{*字符计数器*}
      lc:         integer;     { program location counter }{*程序位置计数器*}
      ll:         integer;     { length of current line }{*当前行长度*}
      errpos:     integer;
      t,a,b,sx,c1,c2:integer;  { indices to tables }{*表索引*}
      iflag, oflag, skipflag, stackdump, prtables: boolean;
      sy:         symbol;      { last symbol read by insymbol }{*由insymbol读取的最后一个符号*}
      errs:       set of 0..ermax;{*包含0到58的集合*}
      id:         alfa;        { identifier from insymbol }{*来自insymbol的标识符char id[10]*}
      progname:   alfa;{*char progname[10]*}
      stantyps:   typset;{*包含types元素的集合*}
      constbegsys, typebegsys, blockbegsys, facbegsys, statbegsys: symset;{*包含symbol元素的集合*}
      line:       array[1..llng] of char;{*char line[121]*}
      key:        array[1..nkw] of alfa;{*char key[1...27][1...10]*}
      ksy:        array[1..nkw] of symbol;{*symbol ksy[1...27]*}
      sps:        array[char]of symbol;  { special symbols }{*特殊符号*}
      display:    array[0..lmax] of integer;{*int display[1..7]*}
      tab:        array[0..tmax] of      { indentifier lable }{*标识符标签tab[100]*}
                 packed record
                    name: alfa;
                    link: index;
                    obj:  objecttyp;
                    typ:  types;
                    ref:  index;
                    normal: boolean;
                    lev:  0..lmax;
                    adr: integer
      end;
     atab:       array[1..amax] of    { array-table }{*数组表*}
                 packed record
                     inxtyp,eltyp: types;{数组类型}
                     elref,low,high,elsize,size: index{*赋值为-32767到32767*}
                 end;
     btab:       array[1..bmax] of    { block-table }
                 packed record
                     last, lastpar, psize, vsize: index
                 end;
     stab:       packed array[0..smax] of char; { string table }
     rconst:     array[1..c2max] of real;
     code:       array[0..cmax] of order;
     psin,psout,prr,prd:text;      { default in pascal p }{*四个文件*}
     inf, outf, fprr: string;

procedure errormsg;{*打印被编译源程序中出错信息的摘要*}
  var k : integer;
     msg: array[0..ermax] of alfa;{*char msg[58][10]*}
  begin
    msg[0] := 'undef id  ';    msg[1] := 'multi def ';
    msg[2] := 'identifier';    msg[3] := 'program   ';
msg[4] := ')         ';    msg[5] := ':         ';
    msg[6] := 'syntax    ';    msg[7] := 'ident,var '; 
msg[8] := 'of        ';    msg[9] := '(         ';
    msg[10] := 'id,array  ';    msg[11] := '(         ';
    msg[12] := ']         ';    msg[13] := '..        ';
    msg[14] := ';         ';    msg[15] := 'func. type';
    msg[16] := '=         ';    msg[17] := 'boolean   ';
    msg[18] := 'convar typ';    msg[19] := 'type      ';
    msg[20] := 'prog.param';    msg[21] := 'too big   ';
    msg[22] := '.         ';    msg[23] := 'type(case)';
    msg[24] := 'character ';    msg[25] := 'const id  ';
    msg[26] := 'index type';    msg[27] := 'indexbound';
    msg[28] := 'no array  ';    msg[29] := 'type id   ';
    msg[30] := 'undef type';    msg[31] := 'no record ';
    msg[32] := 'boole type';    msg[33] := 'arith type';
msg[34] := 'integer   ';    msg[35] := 'types     ';
    msg[36] := 'param type';    msg[37] := 'variab id ';
msg[38] := 'string    ';    msg[39] := 'no.of pars';
    msg[40] := 'real numbr';    msg[41] := 'type      ';
msg[42] := 'real type ';    msg[43] := 'integer   ';
    msg[44] := 'var,const ';    msg[45] := 'var,proc  ';
msg[46] := 'types(:=) ';    msg[47] := 'typ(case) ';
    msg[48] := 'type      ';    msg[49] := 'store ovfl';
    msg[50] := 'constant  ';    msg[51] := ':=        ';
    msg[52] := 'then      ';    msg[53] := 'until     ';
    msg[54] := 'do        ';    msg[55] := 'to downto ';
    msg[56] := 'begin     ';    msg[57] := 'end       ';
    msg[58] := 'factor';{*定义了59种错误*}

    writeln(psout);{*psout.txt打印换行符*}
    writeln(psout,'key words');{*输出'key words'到psout.txt*}
    k := 0;
    while errs <> [] do
      begin
        while not( k in errs )do k := k + 1;
          writeln(psout, k, ' ', msg[k] );
          errs := errs - [k]
    end { while errs }{*将errs集合存储的错误信息按行输出到psout.txt中*}
end { errormsg } ;

procedure endskip;{*源程序出错后在被跳的部分下面印出下划线标志*}
  begin                 { underline skipped part of input }
    while errpos < cc do{*当出错位置没到字符计数器记录为之前一直输出下划线*}
      begin
        write( psout, '-');
        errpos := errpos + 1
      end;
    skipflag := false{*输出结束后跳转信号变为false*}
  end { endskip };


procedure nextch;  { read next character; process line end }{*读下一个字符，处理行结束符，印出被编译的源程序*}
  begin
    if cc = ll{*如果字符计数器位置等于当前行长度*}
    then begin
           if eof( psin ){*如果psin.txt已经被读完*}
           then begin
                  writeln( psout );{*打印换行符*}
                  writeln( psout, 'program incomplete' );{*打印'program incomplete'，即程序未完成*}
                  errormsg;{*输出产生的错误信息*}
                  exit;{*因为读到最后一行了，退出当前程序块*}
                end;
           if errpos <> 0{*如果出错位置不为0，即有错误产生*}
           then begin
                  if skipflag then endskip;{*在有跳转的信号时在被跳的部分下面印出下划线标志*}
                  writeln( psout );{*打印换行符*}
                  errpos := 0{*出错位置归零*}
                end;
           write( psout, lc: 5, ' ');{*输出位宽为5的程序位置计数*}
           ll := 0;{*当前行长度归0*}
           cc := 0;{*字符计数器位置归0*}
           while not eoln( psin ) do{*在这一行没有读到末尾之前一直向psout.txt中打印被编译的源程序*}
             begin
               ll := ll + 1;{*当前行长度加1*}
               read( psin, ch );{*读psin.txt中的一个字符*}
               write( psout, ch );{*将字符写入psout.txt*}
               line[ll] := ch{*记录这一行的每一个字符*}
             end;
           ll := ll + 1;{*当前行长度加1*}
           readln( psin );{*读取psin.txt的新一行*}
           line[ll] := ' ';{*记录psin.txt上一行的数组末尾加空格*}
           writeln( psout );{*打印换行符  *}
         end;
         cc := cc + 1;
         ch := line[cc];
  end { nextch };

procedure error( n: integer );{*打印出错位置和出错编号*}
begin
  if errpos = 0
  then write ( psout, '****' );
  if cc > errpos
  then begin
         write( psout, ' ': cc-errpos, '^', n:2);
errpos := cc + 3;
         errs := errs +[n]
end
end { error };

procedure fatal( n: integer );{*打印表格溢出信息*}
  var msg : array[1..7] of alfa;
  begin
    writeln( psout );{*打印换行符*}
    errormsg;{*打印错误信息*}
    msg[1] := 'identifier';   msg[2] := 'procedures';
    msg[3] := 'reals     ';   msg[4] := 'arrays    ';
msg[5] := 'levels    ';   msg[6] := 'code      ';
    msg[7] := 'strings   ';
writeln( psout, 'compiler table for ', msg[n], ' is too small');
    exit; {terminate compilation }
  end { fatal };

procedure insymbol;  {reads next symbol}{*读取下一单词符号，处理注释行*}
label 1,2,3;{*定义目标1、2、3*}
  var  i,j,k,e: integer;
procedure readscale;{*处理实数的指数部分*}
    var s,sign: integer;{*sign记录实数的正负*}
    begin
      nextch;
      sign := 1;
      s := 0;
      if ch = '+'
      then nextch
      else if ch = '-'
           then begin
                  nextch;
                  sign := -1
                end;
      if not(( ch >= '0' )and (ch <= '9' )){*如果不满足实数要求，产生编号为40的错误*}
      then error( 40 )
      else repeat{*满足实数要求则继续读到不再是0至9之间的字符位置*}
           s := 10*s + ord( ord(ch)-ord('0'));
           nextch;
           until not(( ch >= '0' ) and ( ch <= '9' ));
      e := s*sign + e
    end { readscale };

  procedure adjustscale;{*根据小数位数和指数大小求出实数数值*}
    var s : integer;
        d, t : real;
    begin
      if k + e > emax{*如果大于实数的最大指数则产生编号为21的错误*}
      then error(21)
      else if k + e < emin{*如果小于实数的最小指数则可认为实数为0了*}
           then rnum := 0
           else begin
                  s := abs(e);{*取e得绝对值*}
                  t := 1.0;
                  d := 10.0;
                  repeat
                    while not odd(s) do{*计算指数部分并传给t*}
                      begin
                        s := s div 2;
                        d := sqr(d)
                      end;
                    s := s - 1;
                    t := d * t
                  until s = 0;
                  if e >= 0
                  then rnum := rnum * t
                  else rnum := rnum / t
    end
    end { adjustscale };

  procedure options;{*处理编译时的可选项*}
    procedure switch( var b: boolean );{*处理编译可选项中的‘+’‘-’标志*}
      begin
        b := ch = '+';
        if not b{*如果ch不是‘+’*}
        then if not( ch = '-' ){*如果ch不是‘-’也不是‘+’，则看下一个注释内容直到注释末尾*}
             then begin { print error message }
                    while( ch <> '*' ) and ( ch <> ',' ) do
                      nextch;
                  end
             else nextch
        else nextch
      end { switch };
    begin { options  }
      repeat
        nextch;
        if ch <> '*'{*在遇到注释行结尾前*}
        then begin
               if ch = 't'{*t+或t-确定是否打印表格*}
               then begin
                      nextch;
                      switch( prtables )
                    end
               else if ch = 's'{*s+或s-确定是否进行现场卸出打印*}
                  then begin
                          nextch;
                          switch( stackdump )
                       end;

             end
      until ch <> ','
    end { options };
  begin { insymbol  }
  1: while( ch = ' ' ) or ( ch = chr(9) ) do{*过滤空格或水平制表符*}
       nextch;    { space & htab }
    case ch of
      'a','b','c','d','e','f','g','h','i',
      'j','k','l','m','n','o','p','q','r',
      's','t','u','v','w','x','y','z':
        begin { identifier of wordsymbol }
          k := 0;
          id := '          ';
          repeat
            if k < alng{*用数组id记录接下来的至多10个字符*}
            then begin
                   k := k + 1;
                   id[k] := ch
                 end;
            nextch
          until not((( ch >= 'a' ) and ( ch <= 'z' )) or (( ch >= '0') and (ch <= '9' )));
          i := 1;
          j := nkw; { binary search }{*二分法差关键字表，如果在表中则sy等于该关键字，否则定为ident标识符*}
          repeat
            k := ( i + j ) div 2;
            if id <= key[k]
            then j := k - 1;
            if id >= key[k]
            then i := k + 1;
          until i > j;
          if i - 1 > j
          then sy := ksy[k]
          else sy := ident
        end;
      '0','1','2','3','4','5','6','7','8','9':{*如果以数字开头，说明该字符串只能是数字*}
        begin { number }
          k := 0;
          inum := 0;
          sy := intcon;
          repeat
            inum := inum * 10 + ord(ch) - ord('0');
            k := k + 1;
            nextch
          until not (( ch >= '0' ) and ( ch <= '9' ));
          if( k > kmax ) or ( inum > nmax ){*如果该数长度大于有效数字最大长度或者数值大于实数最大值则报错*}
          then begin
                 error(21);
                 inum := 0;
                 k := 0
               end;
          if ch = '.'{*实数后是否有小数部分*}
          then begin
                 nextch;
                 if ch = '.'
                 then ch := ':'{*连续两个小数点说明是数域*}
                 else begin
                        sy := realcon;
                        rnum := inum;
                        e := 0;
                        while ( ch >= '0' ) and ( ch <= '9' ) do
                        begin
                            e := e - 1;
                            rnum := 10.0 * rnum + (ord(ch) - ord('0'));
                            nextch
                        end;
                        if e = 0
                        then error(40);
                        if ch = 'e'
                        then readscale;
                        if e <> 0 then adjustscale{*得到实数非指数部分和指数部分并求得实数的值*}
                      end
                end
          else if ch = 'e'{*实数后没有小数部分直接是指数部分*}
               then begin
                      sy := realcon;
                      rnum := inum;
                      e := 0;
                      readscale;
                      if e <> 0
                      then adjustscale{*得到实数非指数部分和指数部分并求得实数的值*}
                    end;
        end;
      ':':
        begin
          nextch;
          if ch = '='
          then begin
                 sy := becomes;{*如果是：=，则是赋值符号，如果是：，则是引号*}
                 nextch
               end
          else  sy := colon
         end;
      '<':{*判断是<=或<>或<*}
        begin
          nextch;
          if ch = '='
          then begin
                 sy := leq;
                 nextch
               end
          else
            if ch = '>'
            then begin
                   sy := neq;
                   nextch
                 end
            else  sy := lss
        end;
      '>':{*判断是>或>=*}
        begin
          nextch;
          if ch = '='
          then begin
                 sy := geq;
                 nextch
               end
          else  sy := gtr
        end;
      '.':{*判断是.还是..*}
        begin
          nextch;
          if ch = '.'
          then begin
                 sy := colon;
                 nextch
               end
          else sy := period
        end;
      '''':{*判断引号后面是什么*}
        begin
          k := 0;
   2:     nextch;
          if ch = ''''
          then begin
                 nextch;{*如果是‘’则是空字符*}
                 if ch <> ''''{*如果字符串不为空，*}
                 then goto 3
               end;
          if sx + k = smax{*表格溢出，打印溢出信息*}
          then fatal(7);
          stab[sx+k] := ch;{*没有表格溢出，记录进字符串表*}
          k := k + 1;
          if cc = 1{*这一行只有1个字符的话，字符串长度为0*}
          then begin { end of line }
                 k := 0;
               end
          else goto 2;
   3:     if k = 1
          then begin
                 sy := charcon;{*如果字符串只有一个字符，则类型记为charcon*}
                 inum := ord( stab[sx] ){*将字符转为assic码*}
               end
          else if k = 0
               then begin
                      error(38);
                      sy := charcon;
                      inum := 0{*记录为空字符，但是会报错*}
                    end
               else begin
                      sy := stringcon;{*字符串多于一个字符，类型记为stringcon*}
                      inum := sx;{*记录字符串开始位置*}
                      sleng := k;{*记录字符串长度*}
                      sx := sx + k{*记录字符串结束位置*}
                    end
        end;
      '(':
        begin
          nextch;
          if ch <> '*'
          then sy := lparent{*只是简单的括号*}
          else begin { comment }{*处理编译可选项*}
                 nextch;
                 if ch = '$'
                 then options;
                 repeat
                   while ch <> '*' do nextch;
                   nextch
                 until ch = ')';
                 nextch;
                 goto 1{*一个编译可选项处理完毕，继续处理注释中的其他内容*}
               end
        end;
      '{':
        begin
          nextch;
          if ch = '$'{*处理编译可选项*}
          then options;
          while ch <> '}' do
            nextch;
          nextch;
          goto 1{*一个编译可选项处理完毕，继续处理注释中的其他内容*}
        end;
      '+', '-', '*', '/', ')', '=', ',', '[', ']', ';':
        begin
          sy := sps[ch];{*类型为特殊符号*}
          nextch
        end;
      '$','"' ,'@', '?', '&', '^', '!':
        begin
          error(24);{*打印错误信息*}
          nextch;
          goto 1
        end
      end { case }
    end { insymbol };

procedure enter(x0:alfa; x1:objecttyp; x2:types; x3:integer );{*在分程序外，把标准类型、过程和函数的名字登录到符号表（tab）中*}
  begin
    t := t + 1;    { enter standard identifier }
    with tab[t] do
      begin
        name := x0;{*标识符名字*}
        link := t - 1;{*指向同一个分程序中上一个标识符的位置*}
        obj := x1;{*标识符种类*}
        typ := x2;{*标识符类型*}
        ref := 0;{*属于其他情况，ref一律为0*}
        normal := true;{*不是变量形参，normal为true*}
        lev := 0;{*为第一层，嵌套数为0*}
        adr := x3;
      end
  end; { enter }

procedure enterarray( tp: types; l,h: integer );{*登录数组信息向量表*}
  begin
    if l > h
    then error(27);{*数组越界*}
    if( abs(l) > xmax ) or ( abs(h) > xmax )
    then begin
           error(27);{*数组越界*}
           l := 0;
           h := 0;
         end;
    if a = amax{*数组数目*}
    then fatal(4){*数组表溢出*}
    else begin
           a := a + 1;
           with atab[a] do
             begin{*记录数组信息*}
               inxtyp := tp;
               low := l;
               high := h
             end
         end
  end { enterarray };

procedure enterblock;{*登录分程序表*}
  begin
    if b = bmax
    then fatal(2){*分程序表溢出*}
    else begin{*记录分程序信息*}
           b := b + 1;
           btab[b].last := 0;
           btab[b].lastpar := 0;
         end
  end { enterblock };

procedure enterreal( x: real );{*登录实常数表*}
  begin
    if c2 = c2max - 1{*实常数表长度*}
    then fatal(3){*实常数表溢出*}
    else begin{*记录实常数信息*}
           rconst[c2+1] := x;
           c1 := 1;
           while rconst[c1] <> x do
             c1 := c1 + 1;
           if c1 > c2
           then  c2 := c1{*更新实常数表长度*}
         end
  end { enterreal };

procedure emit( fct: integer );{*生成P代码指令行代码长度*}
  begin
    if lc = cmax
    then fatal(6);{*代码长度溢出*}
    code[lc].f := fct;{*记录当前行代码长度*}
    lc := lc + 1{*转至下一行*}
end { emit };


procedure emit1( fct, b: integer );{*生成P代码指令行代码长度和位宽*}
  begin
    if lc = cmax
    then fatal(6);{*代码长度溢出*}
    with code[lc] do
      begin
        f := fct;
        y := b;
      end;
    lc := lc + 1
  end { emit1 };

procedure emit2( fct, a, b: integer );{*生成P代码指令代码长度、位宽和范围*}
  begin
    if lc = cmax then fatal(6);{*代码长度溢出*}
    with code[lc] do
      begin
        f := fct;
        x := a;
        y := b
      end;
    lc := lc + 1;
end { emit2 };

procedure printtables;{*打印编译生成的符号表、分程序表、实常数表以及P代码*}
  var  i: integer;
      o: order;
      mne: array[0..omax] of
           packed array[1..5] of char;{*char mne[0..63][1..5]*}
  begin
    mne[0] := 'LDA  ';   mne[1] := 'LOD  ';  mne[2] := 'LDI  ';
mne[3] := 'DIS  ';   mne[8] := 'FCT  ';  mne[9] := 'INT  ';
    mne[10] := 'JMP  ';   mne[11] := 'JPC  ';  mne[12] := 'SWT  ';
    mne[13] := 'CAS  ';   mne[14] := 'F1U  ';  mne[15] := 'F2U  ';
    mne[16] := 'F1D  ';   mne[17] := 'F2D  ';  mne[18] := 'MKS  ';
    mne[19] := 'CAL  ';   mne[20] := 'IDX  ';  mne[21] := 'IXX  ';
    mne[22] := 'LDB  ';   mne[23] := 'CPB  ';  mne[24] := 'LDC  ';
mne[25] := 'LDR  ';   mne[26] := 'FLT  ';  mne[27] := 'RED  ';
mne[28] := 'WRS  ';   mne[29] := 'WRW  ';  mne[30] := 'WRU  ';
    mne[31] := 'HLT  ';   mne[32] := 'EXP  ';  mne[33] := 'EXF  ';
    mne[34] := 'LDT  ';   mne[35] := 'NOT  ';  mne[36] := 'MUS  ';
mne[37] := 'WRR  ';   mne[38] := 'STO  ';  mne[39] := 'EQR  ';
mne[40] := 'NER  ';   mne[41] := 'LSR  ';  mne[42] := 'LER  ';
    mne[43] := 'GTR  ';   mne[44] := 'GER  ';  mne[45] := 'EQL  ';
mne[46] := 'NEQ  ';   mne[47] := 'LSS  ';  mne[48] := 'LEQ  ';
    mne[49] := 'GRT  ';   mne[50] := 'GEQ  ';  mne[51] := 'ORR  ';
    mne[52] := 'ADD  ';   mne[53] := 'SUB  ';  mne[54] := 'ADR  ';
    mne[55] := 'SUR  ';   mne[56] := 'AND  ';  mne[57] := 'MUL  ';
    mne[58] := 'DIV  ';   mne[59] := 'MOD  ';  mne[60] := 'MUR  ';
    mne[61] := 'DIR  ';   mne[62] := 'RDL  ';  mne[63] := 'WRL  ';{*记录64种P代码指令*}

writeln(psout);
    writeln(psout);{*打印空行*}
    writeln(psout);
    writeln(psout,'   identifiers  link  obj  typ  ref  nrm  lev  adr');
    writeln(psout);
    for i := btab[1].last to t do
      with tab[i] do
        writeln( psout, i,' ', name, link:5, ord(obj):5, ord(typ):5,ref:5, ord(normal):5,lev:5,adr:5);{*打印编译生成的符号表*}
    writeln( psout );
    writeln( psout );
    writeln( psout );
    writeln( psout, 'blocks   last  lpar  psze  vsze' );
    writeln( psout );
    for i := 1 to b do
       with btab[i] do
         writeln( psout, i:4, last:9, lastpar:5, psize:5, vsize:5 );{*打印编译生成的分程序表*}
    writeln( psout );
    writeln( psout );
    writeln( psout );
    writeln( psout, 'arrays xtyp etyp eref low high elsz size');
    writeln( psout );
    for i := 1 to a do
      with atab[i] do
        writeln( psout, i:4, ord(inxtyp):9, ord(eltyp):5, elref:5, low:5, high:5, elsize:5, size:5);{*打印生成的数组信息向量表*}
    writeln( psout );
    writeln( psout );
    writeln( psout );
    writeln( psout, 'code:');
    writeln( psout );
    for i := 0 to lc-1 do
      begin
write( psout, i:5 );
        o := code[i];
write( psout, mne[o.f]:8, o.f:5 );
        if o.f < 31
        then if o.f < 4
             then write( psout, o.x:5, o.y:5 )
             else write( psout, o.y:10 )
        else write( psout, '          ' );
        writeln( psout, ',' )
      end;
    writeln( psout );
    writeln( psout, 'Starting address is ', tab[btab[1].last].adr:5 )
  end { printtables };


procedure block( fsys: symset; isfun: boolean; level: integer );{*分析处理分程序*}
  type conrec = record
                  case tp: types of
                    ints, chars, bools : ( i:integer );
                    reals :( r:real )
                end;
  var dx : integer ;  { data allocation index }
      prt: integer ;  { t-index of this procedure }
      prb: integer ;  { b-index of this procedure }
      x  : integer ;

  procedure skip( fsys:symset; n:integer);{*跳读源程序，直至取来的符号属于给出的符号集为止，并打印出错标志*}
    begin
      error(n);
      skipflag := true;{*跳读标志变为true*}
      while not ( sy in fsys ) do{*如果符号类型不存在于symbol集合中，*}
        insymbol;{*读取下一符号*}
      if skipflag then endskip{*在出错位置下面打印下划线*}
    end { skip };

  procedure test( s1,s2: symset; n:integer );{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
    begin
      if not( sy in s1 )
      then skip( s1 + s2, n )
    end { test };

  procedure testsemicolon;{*测试当前符号是否为分号*}
    begin
      if sy = semicolon{*如果为分号，读取下一符号*}
      then insymbol
      else begin
             error(14);{*缺少分号报错处理*}
             if sy in [comma, colon]{*如果是逗号或冒号，读下一字符*}
             then insymbol
           end;
      test( [ident] + blockbegsys, fsys, 6 ){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
    end { testsemicolon };


  procedure enter( id: alfa; k:objecttyp );{*在分程序内，在符号表中登录分程序说明部分出现的名字*}
    var j,l : integer;
    begin
      if t = tmax
      then fatal(1){*表溢出*}
      else begin
             tab[0].name := id;{*标识符名字*}
             j := btab[display[level]].last;{*该分程序中说明的最后一个标识符在tab表中的位置*}
             l := j;
             while tab[j].name <> id do{*tab表中没有存该标识符*}
               j := tab[j].link;{*上一个标识符在tab表中的位置*}
             if j <> 0
             then error(1){*重定义错误*}
             else begin
                    t := t + 1;
                    with tab[t] do{*记录该标识符在tab表中的信息*}
                      begin
                        name := id;
                        link := l;
                        obj := k;
                        typ := notyp;
                        ref := 0;
                        lev := level;
                        adr := 0;
                        normal := false { initial value }
                      end;
                    btab[display[level]].last := t{*该分程序中说明的最后一个标识符在tab表中的位置*}
                  end
           end
    end { enter };

  function loc( id: alfa ):integer;{*查找标识符在符号表中的位置*}
    var i,j : integer;        { locate if in table }
    begin
      i := level;{*嵌套程度*}
      tab[0].name := id;  { sentinel }
      repeat{*在符号表中由下向上查找，直到查完第一层或查不到*}
        j := btab[display[i]].last;
        while tab[j].name <> id do
          j := tab[j].link;
          i := i - 1;
      until ( i < 0 ) or ( j <> 0 );
      if j = 0
      then error(0);{*无定义错误*}
      loc := j
    end { loc } ;

  procedure entervariable;{*将变量名登录到符号表中*}
    begin
      if sy = ident{*如果标识符种类为变量，则*}
      then begin
             enter( id, vvariable );{*登录符号表*}
             insymbol
           end
      else error(2){*种类识别错误*}
    end { entervariable };

  procedure constant( fsys: symset; var c: conrec );{*处理程序中出现的常量，并由参数（c）返回该常量的类型和数值*}
    var x, sign : integer;
    begin
      c.tp := notyp;
      c.i := 0;
      test( constbegsys, fsys, 50 );{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
      if sy in constbegsys
      then begin
             if sy = charcon{*如果常量种类为字符型，则记录类型和数值*}
             then begin
                    c.tp := chars;
                    c.i := inum;
                    insymbol
                  end
             else begin
                  sign := 1;
                  if sy in [plus, minus]{*如果当前符号为+或-*}
                  then begin
                         if sy = minus
                         then sign := -1;{*为减号则sign为-*}
                         insymbol{*读下一符号*}
                       end;
                  if sy = ident
                  then begin{*如果常量为标识符，则查找标识符表*}
                         x := loc(id);{*找到在标识符表中的位置*}
                         if x <> 0{*在表中找到改符号*}
                         then
                           if tab[x].obj <> konstant
                           then error(25){*标识符种类错误*}
                           else begin
                                  c.tp := tab[x].typ;
                                  if c.tp = reals{*如果是实型，计算实数值*}
                                  then c.r := sign*rconst[tab[x].adr]
                                  else c.i := sign*tab[x].adr
                                end;
                         insymbol{*读下一符号*}
                       end
                  else if sy = intcon{*如果常量为int型，记录类型和数值*}
                       then begin
                              c.tp := ints;
                              c.i := sign*inum;
                              insymbol
                            end
                  else if sy = realcon{*如果常量为实数，计算实数值*}
                        then begin
                              c.tp := reals;
                              c.r := sign*rnum;
                              insymbol
                            end
                  else skip(fsys,50){*常量不合法，跳读*}
                end;
                test(fsys,[],6){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
           end
    end { constant };

procedure typ( fsys: symset; var tp: types; var rf,sz:integer );{*处理类型描述，由参数得到它的类型（tp），指向类型详细信息表的指针（ref）和该类型大小（sz）*}
    var eltp : types;
        elrf, x : integer;
        elsz, offset, t0, t1 : integer;

procedure arraytyp( var aref, arsz: integer );{*处理数组类型，由参数返回指向该数组信息向量表的指针（aref）和数组大小（arsz）*}
var eltp : types;
      low, high : conrec;
      elrf, elsz: integer;
      begin
        constant( [colon, rbrack, rparent, ofsy] + fsys, low );{*处理程序中出现的常量low，并由参数（c）返回该常量的类型和数值*}
        if low.tp = reals{*如果是实型*}
        then begin
               error(27);{*访问越界错误*}
               low.tp := ints;{*返回类型和数值*}
               low.i := 0
             end;
        if sy = colon{*如果是冒号，读下一个符号*}
        then insymbol
        else error(13);{*其他情况视为错误*}
        constant( [rbrack, comma, rparent, ofsy ] + fsys, high );{*处理程序中出现的常量high，并由参数（c）返回该常量的类型和数值*}
        if high.tp <> low.tp
        then begin
               error(27);{*如果high和low的类型不同，则访问越界处理*}
               high.i := low.i
             end;
        enterarray( low.tp, low.i, high.i );{*登录数组信息向量表*}
        aref := a;
        if sy = comma{*如果sy为逗号，读下一字符，记录类型为数组类型并处理*}
        then begin
               insymbol;
               eltp := arrays;
               arraytyp( elrf, elsz )
             end
        else begin
               if sy = rbrack{*如果sy为]，读下一字符*}
               then insymbol
               else begin
                      error(12);{*数组右方括号不完整错误*}
                      if sy = rparent{*如果sy为rparent，读下一字符*}
                      then insymbol
                    end;
               if sy = ofsy{*如果sy为ofsy，读下一字符*}
               then insymbol
               else error(8);{*缺少ofsy符号错误*}
               typ( fsys, eltp, elrf, elsz ){*处理类型描述，由参数得到它的类型（tp），指向类型详细信息表的指针（ref）和该类型大小（sz）*}
             end;
             with atab[aref] do{*记录数组信息*}
               begin
                 arsz := (high-low+1) * elsz;
                 size := arsz;
                 eltyp := eltp;
                 elref := elrf;
                 elsize := elsz
               end
      end { arraytyp };
    begin { typ  }
      tp := notyp;
      rf := 0;
      sz := 0;
      test( typebegsys, fsys, 10 );{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
      if sy in typebegsys
      then begin
             if sy = ident{*如果sy类型为标识符*}
             then begin
                    x := loc(id);{*找到标识符在符号表中的位置*}
                    if x <> 0
                    then with tab[x] do
                           if obj <> typel
                           then error(29){*标识符类型名错误*}
                           else begin{*类型正确时，记录类型，指针和大小*}
                                  tp := typ;
                                  rf := ref;
                                  sz := adr;
                                  if tp = notyp
                                  then error(30){*无定义错误*}
                                end;
                    insymbol
                  end
             else if sy = arraysy{*如果标识符为数组类型*}
                  then begin
                         insymbol;{*读下一符号*}
                         if sy = lbrack{*sy为左括号时，读下一符号*}
                         then insymbol
                         else begin
                                error(11);{*缺失左括号错误*}
                                if sy = lparent
                                then insymbol
                              end;
                         tp := arrays;{*记录数组类型*}
                         arraytyp(rf,sz){*处理数组类型，由参数返回指向该数组信息向量表的指针（aref）和数组大小（arsz）*}
                         end
             else begin { records }
                    insymbol;{*读下一符号*}
                    enterblock;{*登录分程序表*}
                    tp := records;
                    rf := b;
                    if level = lmax
                    then fatal(5);{*嵌套层数溢出*}
                    level := level + 1;
                    display[level] := b;{*记录嵌套层数和填display索引表*}
                    offset := 0;
                    while not ( sy in fsys - [semicolon,comma,ident]+ [endsy] ) do{*sy是分号，逗号，标识符时*}
                      begin { field section }
                        if sy = ident{*如果sy为标识符*}
                        then begin
                               t0 := t;
                               entervariable;{*将变量名登录符号表中*}
                               while sy = comma do{*当sy为逗号时*}
                                 begin
                                   insymbol;
                                   entervariable{*将变量名登录符号表中*}
                                 end;
                               if sy = colon{*如果sy为冒号*}
                               then insymbol{*读下一符号*}
                               else error(5);{*缺少冒号错误*}
                               t1 := t;
                               typ( fsys + [semicolon, endsy, comma,ident], eltp, elrf, elsz );{*处理类型描述，由参数得到它的类型（tp），指向类型详细信息表的指针（ref）和该类型大小（sz）*}
                               while t0 < t1 do{*记录t1前一个符号表符号的信息*}
                               begin
                                 t0 := t0 + 1;
                                 with tab[t0] do
                                   begin
                                     typ := eltp;
                                     ref := elrf;
                                     normal := true;
                                     adr := offset;
                                     offset := offset + elsz
                                   end
                               end
                             end; { sy = ident }
                        if sy <> endsy{*如果sy为endsy*}
                        then begin
                               if sy = semicolon{*如果sy为分号*}
                               then insymbol{*读下一符号*}
                               else begin
                                      error(14);{*缺失分号错误*}
                                      if sy = comma{*如果sy为逗号，读下一符号*}
                                      then insymbol
                                    end;
                                    test( [ident,endsy, semicolon],fsys,6 ){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
                             end
                      end; { field section }
                    btab[rf].vsize := offset;{*记录分程序表的信息*}
                    sz := offset;
                    btab[rf].psize := 0;
                    insymbol;
                    level := level - 1
                  end; { record }
             test( fsys, [],6 ){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
           end;
      end { typ };

  procedure parameterlist; { formal parameter list  }{*处理过程或函数说明中的形参表，将形参及有关信息登录到符号表中*}
    var tp : types;
        valpar : boolean;
        rf, sz, x, t0 : integer;
    begin
      insymbol;{*读下一符号*}
      tp := notyp;
      rf := 0;
      sz := 0;
      test( [ident, varsy], fsys+[rparent], 7 );{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
      while sy in [ident, varsy] do{*如果当前符号为标识符或者变量*}
        begin
          if sy <> varsy
          then valpar := true{*如果是变量，则valpar赋为true*}
          else begin
                 insymbol;{*读下一符号*}
                 valpar := false{*valpar赋为false*}
               end;
          t0 := t;{*得到符号表中的位置*}
          entervariable;{*将变量名登录符号表中*}
          while sy = comma do{*sy为逗号时*}
            begin
              insymbol;{*读下一符号*}
              entervariable;{*将变量名登录符号表中*}
            end;
          if sy = colon{*sy为冒号时*}
          then begin
                 insymbol;{*读下一符号*}
                 if sy <> ident
                 then error(2){*sy不是标识符时，显示识别错误*}
                 else begin
                        x := loc(id);{*找到标识符在符号表中的位置*}
                        insymbol;{*读下一符号*}
                        if x <> 0
                        then with tab[x] do
                          if obj <> typel
                          then error(29){*类型名错误*}
                          else begin{*记录形参及有关信息*}
                                 tp := typ;
                                 rf := ref;
                                 if valpar
                                 then sz := adr
                                 else sz := 1
                               end;
                      end;
                 test( [semicolon, rparent], [comma,ident]+fsys, 14 ){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
                 end
          else error(5);{*缺失冒号错误*}
          while t0 < t do
            begin
              t0 := t0 + 1;
              with tab[t0] do{*将形参及有关信息登录到符号表中*}
                begin
                  typ := tp;
                  ref := rf;
                  adr := dx;
                  lev := level;
                  normal := valpar;
                  dx := dx + sz
                end
            end;
            if sy <> rparent
            then begin
                   if sy = semicolon{*sy为分号时*}
                   then insymbol{*读下一符号*}
                   else begin
                          error(14);{*单引号缺失错误*}
                          if sy = comma{*sy为逗号时*}
                          then insymbol{*读下一符号*}
                        end;
                        test( [ident, varsy],[rparent]+fsys,6){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
                 end
        end { while };
      if sy = rparent{*sy为右括号时*}
      then begin
             insymbol;{*读下一符号*}
             test( [semicolon, colon],fsys,6 ){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
           end
      else error(4){*缺失右括号错误*}
    end { parameterlist };


procedure constdec;{*处理常量定义，将常量名及其相应信息填入符号表*}
    var c : conrec;
    begin
      insymbol;{*读下一符号*}
      test([ident], blockbegsys, 2 );{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
      while sy = ident do{*当前符号为标识符时*}
        begin
          enter(id, konstant);{*登录符号表中*}
          insymbol;{*读下一符号*}
          if sy = eql{*当前符号为等号时*}
          then insymbol{*读下一符号*}
          else begin
                 error(16);{*缺失等号错误*}
                 if sy = becomes{*当前符号为becomes时*}
                 then insymbol{*读下一符号*}
               end;
          constant([semicolon,comma,ident]+fsys,c);{*处理程序中出现的常量，并由参数（c）返回该常量的类型和数值*}
          tab[t].typ := c.tp;{*将参数c返回的常量类型和数值写入符号表*}
          tab[t].ref := 0;
          if c.tp = reals{*如果常量类型为实型*}
          then begin
                enterreal(c.r);{*登录实常数表*}
                tab[t].adr := c1;{*将实常数在实常数表中的登入位置写入符号表*}
              end
          else tab[t].adr := c.i;
          {*测试当前符号是否为分号*}
      end
    end { constdec };

  procedure typedeclaration;{*处理类型定义，并将类型名及其相应信息填入符号表*}
var tp: types;
        rf, sz, t1 : integer;
begin
      insymbol;{*读下一符号*}
      test([ident], blockbegsys,2 );{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
      while sy = ident do{*当前符号为标识符时*}
        begin
          enter(id, typel);{*登入符号表*}
          t1 := t;{*记录符号表中最后一个符号的位置*}
          insymbol;{*读下一符号*}
          if sy = eql{*当期符号为等号时*}
          then insymbol{*读下一符号*}
          else begin
                 error(16);{*缺失等号错误*}
                 if sy = becomes{*当前符号为becomes时*}
                 then insymbol{*读下一符号*}
               end;
          typ( [semicolon,comma,ident]+fsys, tp,rf,sz );{*处理类型描述，由参数得到它的类型（tp），指向类型详细信息表的指针（ref）和该类型大小（sz）*}
          with tab[t1] do{*将类型名及其相应信息填入符号表*}
            begin
              typ := tp;
              ref := rf;
              adr := sz
            end;
          testsemicolon{*测试当前符号是否为分号*}
        end
    end { typedeclaration };

  procedure variabledeclaration;{*处理变量定义，将变量名及其相应信息填入符号表*}
    var tp : types;
        t0, t1, rf, sz : integer;
    begin
      insymbol;{*读下一符号*}
    while sy = ident do{*当前符号为标识符时*}
        begin
          t0 := t;
          entervariable;{*将变量名登录到符号表中*}
          while sy = comma do{*当前符号为逗号时*}
            begin
              insymbol;
              entervariable;{*将变量名登录到符号表中*}
            end;
          if sy = colon{*当前符号为冒号*}
          then insymbol{*读下一符号*}
          else error(5);{*冒号缺失错误*}
          t1 := t;
          typ([semicolon,comma,ident]+fsys, tp,rf,sz );{*处理类型描述，由参数得到它的类型（tp），指向类型详细信息表的指针（ref）和该类型大小（sz）*}
          while t0 < t1 do
            begin
              t0 := t0 + 1;
              with tab[t0] do{*将变量名及其相应信息填入符号表*}
                begin
                  typ := tp;
                  ref := rf;
                  lev := level;
                  adr := dx;
                  normal := true;
                  dx := dx + sz
                end
            end;
          testsemicolon{*测试当前符号是否为分号*}
        end
    end { variabledeclaration };

  procedure procdeclaration;{*处理过程或函数声明，将过程（函数）名填入符号表，递归调用block分析处理程序（层次：level+1）*}
    var isfun : boolean;
    begin
      isfun := sy = funcsy;{*当前符号为函数或过程名时isfun为1*}
      insymbol;{*读下一符号*}
      if sy <> ident{*如果当前符号不是标识符，显示识别错误*}
      then begin
             error(2);
             id :='          '
           end;
      if isfun{*当前符号为函数或过程名*}
      then enter(id,funktion){*登入符号表*}
      else enter(id,prozedure);{*登入符号表*}
      tab[t].normal := true;
      insymbol;{*读下一符号*}
      block([semicolon]+fsys, isfun, level+1 );{*分析处理分程序*}
      if sy = semicolon{*当前符号为分号*}
      then insymbol{*读下一符号*}
      else error(14);{*引号缺失错误*}
      emit(32+ord(isfun)) {exit}
    end { proceduredeclaration };


procedure statement( fsys:symset );{*分析处理各种语句*}
    var i : integer;

procedure expression(fsys:symset; var x:item); forward;{*分析处理表达式，由参数（x）返回求值*}
    procedure selector(fsys:symset; var v:item);{*处理结构变量：数组下标变量或记录成员变量*}
    var x : item;
        a,j : integer;
    begin { sy in [lparent, lbrack, period] }
      repeat
        if sy = period{*如果读到句号*}
        then begin
               insymbol; { field selector }{*读下一符号*}
               if sy <> ident{*如果当前符号不是标识符，显示识别错误*}
               then error(2)
               else begin
                      if v.typ <> records{*如果v的类型不是记录型，显示没有记录错误类型*}
                      then error(31)
                      else begin { search field identifier }{*搜索字段标识符*}
                             j := btab[v.ref].last;{*查找分程序表中的上一个标识符的位置*}
                             tab[0].name := id;
                             while tab[j].name <> id do
                               j := tab[j].link;
                             if j = 0{*j=0，说明查不到，报错无定义*}
                             then error(0);
                             v.typ := tab[j].typ;{*如果查到了，记录信息*}
                             v.ref := tab[j].ref;
                             a := tab[j].adr;
                             if a <> 0
                             then emit1(9,a){*生成P代码指令行代码长度和位宽*}
                           end;
                      insymbol{*读下一符号*}
                    end
             end
        else begin { array selector }{*数组选择器*}
               if sy <> lbrack{*当前符号不为左括号，显示左括号缺失错误*}
               then error(11);
               repeat
                 insymbol;{*读下一符号*}
                 expression( fsys+[comma,rbrack],x);{*分析处理表达式，由参数（x）返回求值*}
                 if v.typ <> arrays{*如果v的类型不是数组型，显示无数组错误*}
                 then error(28)
                 else begin
                        a := v.ref;{**数组v在数组信息向量表中的登录位置}
                        if atab[a].inxtyp <> x.typ{*如果v数组下标索引类型不同，则显示索引类型错误*}
                        then error(26)
                        else if atab[a].elsize = 1{*如果数组v数组元素大小为1*}
                             then emit1(20,a){*生成P代码指令行代码长度和位宽*}
                             else emit1(21,a);{*生成P代码指令行代码长度和位宽*}
                        v.typ := atab[a].eltyp;
                        v.ref := atab[a].elref
                      end
               until sy <> comma;{*读到逗号时结束*}
               if sy = rbrack{*当前符号为右方括号时，读下一个字符*}
               then insymbol
               else begin
                      error(12);{*缺失右方括号错误*}
                      if sy = rparent{*如果为右括号，则读下一个字符*}
                      then insymbol
                    end
             end
      until not( sy in[lbrack, lparent, period]);{*知道当前符号是）或]或.*}
      test( fsys,[],6){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
    end { selector };

    procedure call( fsys: symset; i:integer );{*处理非标准的过程或函数调用*}
        var x : item;
          lastp,cp,k : integer;
        begin
        emit1(18,i); { mark stack }{*标记堆栈*}
        lastp := btab[tab[i].ref].lastpar;
        cp := i;
        if sy = lparent
        then begin { actual parameter list }
               repeat
                 insymbol;{*读下一个字符*}
                 if cp >= lastp
                 then error(39){*如果cp大于btab中最后一个标识符在tab表中的位置，则显示参数数量错误*}
                 else begin
                        cp := cp + 1;
                        if tab[cp].normal
                        then begin { value parameter }{*tab[cp]非变量形参时*}
                               expression( fsys+[comma, colon,rparent],x);{*分析处理表达式，由参数（x）返回求值*}
                               if x.typ = tab[cp].typ
                               then begin
                                      if x.ref <> tab[cp].ref
                                      then error(36){*参数和符号表中的参数类型不同时，显示参数类型错误*}
                                      else if x.typ = arrays
                                           then emit1(22,atab[x.ref].size){*如果参数为数组类型，生成P代码指令行代码长度为22，位宽为数组大小*}
                                           else if x.typ = records
                                                then emit1(22,btab[x.ref].vsize){*如果参数为记录类型，生成P代码指令行代码长度为22，位宽为相关信息在运行栈S中的存储单元数*}
                                    end
                               else if ( x.typ = ints ) and ( tab[cp].typ = reals )
                                    then emit1(26,0){*如果x类型为整型，tab[cp]为实型，生成P代码指令行代码长度为26，位宽为0*}
                                    else if x.typ <> notyp
                                         then error(36);{*如果x还有其他类型，显示参数类型错误*}
                             end
                        else begin { variable parameter }{*tab[cp]为变量参数时*}
                               if sy <> ident{*如果当前符号为标识符。显示识别错误*}
                               then error(2)
                               else begin
                                      k := loc(id);{*找到标识符在符号表中的位置*}
                                      insymbol;{*读下一符号*}
                                      if k <> 0{*标识符存在于符号表中时*}
                                      then begin
                                             if tab[k].obj <> vvariable{*如果标识符种类不为变量，显示类型错误*}
                                             then error(37);
                                             x.typ := tab[k].typ;
                                             x.ref := tab[k].ref;{*记录标识符的类型和指向在btab表中的指针*}
                                             if tab[k].normal{*如果标识符不是变量形参*}
                                             then emit2(0,tab[k].lev,tab[k].adr){*生成P代码指令代码长度、位宽和范围为0*}{*生成P代码指令代码长度、位宽和范围为0*}
                                              else emit2(1,tab[k].lev,tab[k].adr);{*生成P代码指令代码长度、位宽和范围为1*}
                                              if sy in [lbrack, lparent, period]{*当前符号为（，[或.类型时*}
                                             then selector(fsys+[comma,colon,rparent],x);{*处理结构变量：数组下标变量或记录成员变量*}
                                             if ( x.typ <> tab[cp].typ ) or ( x.ref <> tab[cp].ref )
                                             then error(36){*如果x的类型与符号表记录的不一致，显示参数类型错误*}
                                           end
                                    end
                             end {variable parameter }
                      end;
                 test( [comma, rparent],fsys,6){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
               until sy <> comma;
               if sy = rparent{*如果sy不为右括号，读下一符号，显示右括号缺失错误*}
               then insymbol
               else error(4)
             end;
        if cp < lastp{*显示参数数量少错误*}
        then error(39); { too few actual parameters }
        emit1(19,btab[tab[i].ref].psize-1 );{*生成P代码指令行代码长度和位宽*}
        if tab[i].lev < level
        then emit2(3,tab[i].lev, level )
      end { call };

    function resulttype( a, b : types) :types;{*处理整型或实型整型或实型两个操作数运行时的类型转换*}
      begin
        if ( a > reals ) or ( b > reals )
        then begin{*如果操作数不是整型或实型，则显示计算类型出错*}
               error(33);
               resulttype := notyp
             end
        else if ( a = notyp ) or ( b = notyp ){*操作数无类型，则结果无类型*}
             then resulttype := notyp
             else if a = ints
                  then if b = ints
                       then resulttype := ints{*操作数为整型，则结果为整型*}
                       else begin
                              resulttype := reals;{*操作数一个为整型，一个为实型，则结果为实型*}
                              emit1(26,1)
                            end
                  else begin
                         resulttype := reals;
                         if b = ints
                         then emit1(26,0)
                       end
      end { resulttype } ;

    procedure expression( fsys: symset; var x: item );{*分析处理表达式，由参数（x）返回求值*}
var y : item;
          op : symbol;

      procedure simpleexpression( fsys: symset; var x: item );{*处理简单表达式，由参数（x）返回求值结果的类型*}
        var y : item;
            op : symbol;

        procedure term( fsys: symset; var x: item );{*处理项，由参数返回结果类型*}
          var y : item;
              op : symbol;

          procedure factor( fsys: symset; var x: item );{*处理因子，由参数返回结果类型*}
            var i,f : integer;

            procedure standfct( n: integer );{*处理标准函数调用*}
              var ts : typset;
              begin  { standard function no. n }
                if sy = lparent{*当前符号为(*}
                then insymbol{*读下一符号*}
                else error(9);{*缺失左括号错误*}
                if n < 17
                then begin
                       expression( fsys+[rparent], x );{*分析处理表达式，由参数（x）返回求值*}
                       case n of{*由n的值和x的类型，确定调用的函数*}
                       { abs, sqr } 0,2: begin
                                           ts := [ints, reals];
                                           tab[i].typ := x.typ;
                                           if x.typ = reals
                                           then n := n + 1
                                         end;
                       { odd, chr } 4,5: ts := [ints];
                       { odr }        6: ts := [ints,bools,chars];
                       { succ,pred } 7,8 : begin
                                             ts := [ints, bools,chars];
                                             tab[i].typ := x.typ
                                           end;
                       { round,trunc } 9,10,11,12,13,14,15,16:
                       { sin,cos,... }     begin
                                             ts := [ints,reals];
                                             if x.typ = ints
                                             then emit1(26,0){*生成P代码指令行代码长度和位宽*}
                                           end;
                     end; { case }
                     if x.typ in ts
                     then emit1(8,n){*生成P代码指令行代码长度和位宽*}
                     else if x.typ <> notyp
                          then error(48);{*x有其他类型，则显示类型错误*}
                   end
                else begin { n in [17,18] }
                       if sy <> ident
                       then error(2){*当前符号不为标识符，则显示识别错误*}
                       else if id <> 'input    '
                            then error(0){*标识符不是input，显示无定义*}
                            else insymbol;{*读下一符号*}
                       emit1(8,n);{*生成P代码指令行代码长度和位宽*}
                     end;
                x.typ := tab[i].typ;
                if sy = rparent{*当前符号为右括号则读下一符号，否则显示缺失右括号错误*}
                then insymbol
                else error(4)
              end { standfct } ;
            begin { factor }
              x.typ := notyp;
              x.ref := 0;
              test( facbegsys, fsys,58 );{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
              while sy in facbegsys do{*当前符号为symbol集合的元素时*}
                begin
                  if sy = ident{*如果为标识符*}
                  then begin
                         i := loc(id);{*找到标识符在符号表中的位置*}
                         insymbol;{*读下一符号*}
                         with tab[i] do
                           case obj of
                             konstant: begin{*因子种类为常量*}
                                         x.typ := typ;
                                         x.ref := 0;
                                         if x.typ = reals
                                          then emit1(25,adr){*生成P代码指令行代码长度和位宽*}
                                         else emit1(24,adr){*生成P代码指令行代码长度和位宽*}
                                        end;
                             vvariable:begin{*因子种类为变量*}
                                         x.typ := typ;
                                         x.ref := ref;
                                         if sy in [lbrack, lparent,period]{*当前符号为(或[或.*}
                                         then begin
                                                if normal{*如果不是变量形参*}
                                                then f := 0
                                                else f := 1;
                                                emit2(f,lev,adr);{*生成P代码指令代码长度、位宽和范围*}
                                                selector(fsys,x);{*处理结构变量：数组下标变量或记录成员变量*}
                                                if x.typ in stantyps
                                                then emit(34){*生成P代码指令代码长度*}
                                              end
                                         else begin
                                                if x.typ in stantyps
                                                then if normal{*如果不是变量形参*}
                                                     then f := 1
                                                     else f := 2
                                                else if normal{*如果不是变量形参*}
                                                     then f := 0
                                                else f := 1;
                                                emit2(f,lev,adr)
                                          end
                                       end;
                             typel,prozedure: error(44);{*因子种类为类型或过程，显示常量变量错误*}
                             funktion: begin{*因子种类为函数*}
                                         x.typ := typ;
                                         if lev <> 0
                                         then call(fsys,i){*处理非标准的过程或函数调用*}
                                         else standfct(adr){*处理标准函数调用*}
                                       end
                           end { case,with }
                       end
                  else if sy in [ charcon,intcon,realcon ]{*因子为字符常量，整数或实常数时*}
                       then begin
                              if sy = realcon{*因子为实常数*}
                              then begin
                                    x.typ := reals;
                                    enterreal(rnum);{*登录实常数表*}
                                    emit1(25,c1){*生成P代码指令行代码长度和位宽*}
                                   end
                              else begin
                                     if sy = charcon{*因子为字符常量*}
                                     then x.typ := chars
                                     else x.typ := ints;{*因子为整型*}
                                     emit1(24,inum){*生成P代码指令行代码长度和位宽*}
                                   end;
                              x.ref := 0;
                              insymbol{*读下一符号*}
                            end
                       else if sy = lparent{*当前符号为左括号*}
                            then begin
                                   insymbol;{*读下一符号*}
                                   expression(fsys + [rparent],x);{*分析处理表达式，由参数（x）返回求值*}
                                   if sy = rparent{*当前符号为右括号*}
                                   then insymbol{*读下一符号*}
                                   else error(4){*右括号缺失错误*}
                                 end
                             else if sy = notsy{*当前符号不是特殊符号时*}
                                  then begin
                                         insymbol;{*读下一符号*}
                                         factor(fsys,x);
                                         if x.typ = bools
                                         then emit(35){*生成P代码指令代码长度*}
                                         else if x.typ <> notyp
                                              then error(32)
                                       end;
                  test(fsys,facbegsys,6){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
                end { while }
            end { factor };
          begin { term   }
            factor( fsys + [times,rdiv,idiv,imod,andsy],x);{*处理因子，由参数返回结果类型*}
            while sy in [times,rdiv,idiv,imod,andsy] do
              begin
                op := sy;
                insymbol;{*读下一符号*}
                factor(fsys+[times,rdiv,idiv,imod,andsy],y );{*处理因子，由参数返回结果类型*}
                if op = times{*op为乘*}
                then begin
                       x.typ := resulttype(x.typ, y.typ);{*处理整型或实型整型或实型两个操作数运行时的类型转换*}
                       case x.typ of
                         notyp: ;
                         ints : emit(57);{*x为整型，生成P代码指令代码长度*}
                         reals: emit(60);{*x为实型，生成P代码指令代码长度*}
                       end
                     end
                else if op = rdiv{*op为除以*}
                     then begin
                            if x.typ = ints{*x类型为整型*}
                            then begin
                                   emit1(26,1);{*生成P代码指令行代码长度和位宽*}
                                   x.typ := reals;{*整型转实型*}
                                 end;
                            if y.typ = ints{*y类型为整型*}
                            then begin
                                   emit1(26,0);{*生成P代码指令行代码长度和位宽*}
                                   y.typ := reals;{*整型转实型*}
                                 end;
                            if (x.typ = reals) and (y.typ = reals)
                            then emit(61){*生成P代码指令代码长度*}
                            else begin
                                   if( x.typ <> notyp ) and (y.typ <> notyp){*若x，y无类型，显示计算类型错误*}
                                   then error(33);
                                   x.typ := notyp
                                 end
                          end
                     else if op = andsy{*op为与*}
                          then begin
                                 if( x.typ = bools )and(y.typ = bools){*x，y为布尔类型时*}
                                 then emit(56){*生成P代码指令代码长度*}
                                 else begin
                                        if( x.typ <> notyp ) and (y.typ <> notyp){*若x，y无类型，显示计算类型错误*}
                                        then error(32);
                                        x.typ := notyp
                                      end
                               end
                          else begin { op in [idiv,imod] }{*op为除或模时*}
                                 if (x.typ = ints) and (y.typ = ints)
                                 then if op = idiv{*x，y为整型，op为除*}
                                      then emit(58){*生成P代码指令代码长度*}
                                      else emit(59){*生成P代码指令代码长度*}
                                 else begin{*x，y为整型，op为模*}
                                        if ( x.typ <> notyp ) and (y.typ <> notyp){*若x，y无类型，显示计算类型错误*}
                                        then error(34);
                                        x.typ := notyp
                                      end
                               end
              end { while }
          end { term };
        begin { simpleexpression }
          if sy in [plus,minus]{*当前符号为加或减*}
          then begin
                 op := sy;
                 insymbol;{*读下一符号*}
                 term( fsys+[plus,minus],x);{*处理项，由参数返回结果类型*}
                 if x.typ > reals
                 then error(33){*若x类型不是整型或实型，显示计算类型错误*}
                 else if op = minus{*当前符号为减*}
                      then emit(36){*生成P代码指令代码长度*}
               end
          else term(fsys+[plus,minus,orsy],x);{*处理项，由参数返回结果类型*}
          while sy in [plus,minus,orsy] do{*当前符号为加或减或或*}
            begin
              op := sy;
              insymbol;{*读下一符号*}
              term(fsys+[plus,minus,orsy],y);{*处理项，由参数返回结果类型*}
              if op = orsy{*op为或*}
              then begin
                     if ( x.typ = bools )and(y.typ = bools){*x，y为布尔类型时*}
                     then emit(51){*生成P代码指令代码长度*}
                     else begin
                            if( x.typ <> notyp) and (y.typ <> notyp){*若x，y无类型，显示计算类型错误*}
                            then error(32);
                            x.typ := notyp
                          end
                   end
              else begin
                     x.typ := resulttype(x.typ,y.typ);{*处理整型或实型整型或实型两个操作数运行时的类型转换*}
                     case x.typ of
                       notyp: ;{*x无类型*}
                       ints: if op = plus{*x为整型，op为加*}
                             then emit(52){*生成P代码指令代码长度*}
                             else emit(53);{*生成P代码指令代码长度*}
                       reals:if op = plus{*x为实型，op为加*}
                             then emit(54){*生成P代码指令代码长度*}
                             else emit(55){*生成P代码指令代码长度*}
                     end { case }
                   end
            end { while }
          end { simpleexpression };
      begin { expression  }
        simpleexpression(fsys+[eql,neq,lss,leq,gtr,geq],x);{*处理简单表达式，由参数（x）返回求值结果的类型*}
        if sy in [ eql,neq,lss,leq,gtr,geq]
        then begin
               op := sy;
               insymbol;{*读下一符号*}
               simpleexpression(fsys,y);{*处理简单表达式，由参数（x）返回求值结果的类型*}
               if(x.typ in [notyp,ints,bools,chars]) and (x.typ = y.typ){*若x，y类型相同且为无类型或整型或布尔类型或字符型时*}
               then case op of
                      eql: emit(45);{*op为等于*}
                      neq: emit(46);{*op为不等于*}
                      lss: emit(47);{*op为等于lss*}
                      leq: emit(48);{*op为等于leq*}
                      gtr: emit(49);{*op为等于gtr*}
                      geq: emit(50);{*op为等于geq*}
                    end
               else begin
                      if x.typ = ints
                      then begin
                             x.typ := reals;{*x类型为整型时，整型转实型*}
                             emit1(26,1){*生成P代码指令行代码长度和位宽*}
                           end
                      else if y.typ = ints
                           then begin
                                  y.typ := reals;{*y类型为整型时，整型转实型*}
                                  emit1(26,0){*生成P代码指令行代码长度和位宽*}
                                end;
                      if ( x.typ = reals)and(y.typ=reals){*x，y类型为实型时*}
                      then case op of
                             eql: emit(39);{*op为等于*}
                             neq: emit(40);{*op为不等于*}
                             lss: emit(41);{*op为lss*}
                             leq: emit(42);{*op为等于leq*}
                             gtr: emit(43);{*op为等于gtr*}
                             geq: emit(44);{*op为等于geq*}
                           end
                      else error(35){*其他类型时则显示类型错误*}
                    end;
               x.typ := bools
             end
      end { expression };

    procedure assignment( lv, ad: integer );{*处理赋值语句*}
      var x,y: item;
          f  : integer;
      begin   { tab[i].obj in [variable,prozedure] }{*tab[i]的种类为变量或过程时*}
        x.typ := tab[i].typ;
        x.ref := tab[i].ref;
        if tab[i].normal{*tab[i]不为变量形参时*}
        then f := 0
        else f := 1;
        emit2(f,lv,ad);{*生成P代码指令代码长度、位宽和范围*}
        if sy in [lbrack,lparent,period]{*若当前符号为(或[或.*}
        then selector([becomes,eql]+fsys,x);{*处理结构变量：数组下标变量或记录成员变量*}
        if sy = becomes{*当前符号为:=*}
        then insymbol{*读下一符号*}
        else begin
               error(51);{*缺失赋值符号错误*}
               if sy = eql{*若当前符号为=，则读下一符号*}
               then insymbol
             end;
        expression(fsys,y);{*分析处理表达式，由参数（x）返回求值*}
        if x.typ = y.typ
        then if x.typ in stantyps
             then emit(38){*生成P代码指令代码长度*}
             else if x.ref <> y.ref{*x，yref不同时*}
                  then error(46){*类型不等错误*}
                  else if x.typ = arrays{*x类型为数组时*}
                       then emit1(23,atab[x.ref].size)
                       else emit1(23,btab[x.ref].vsize)
        else if(x.typ = reals )and (y.typ = ints)
        then begin
               emit1(26,0);{*生成P代码指令行代码长度和位宽*}
               emit(38){*生成P代码指令行代码长度*}
             end
        else if ( x.typ <> notyp ) and ( y.typ <> notyp ){*x，y还有其他类型时，显示类型赋值错误*}
             then error(46)
      end { assignment };

    procedure compoundstatement;{*处理复合语句*}
      begin
        insymbol;{*读下一符号*}
        statement([semicolon,endsy]+fsys);{*分析处理各种语句*}
        while sy in [semicolon]+statbegsys do
          begin
            if sy = semicolon{*当前符号为引号时*}
            then insymbol{*读下一符号*}
            else error(14);{*引号缺失错误*}
            statement([semicolon,endsy]+fsys){*分析处理各种语句*}
          end;
        if sy = endsy{*当前符号为end时*}
        then insymbol{*读下一符号*}
        else error(57){*否则缺失end错误*}
      end { compoundstatement };

    procedure ifstatement;{*处理if语句*}
      var x : item;
          lc1,lc2: integer;
      begin
        insymbol;{*读下一符号*}
        expression( fsys+[thensy,dosy],x);{*分析处理表达式，由参数（x）返回求值*}
        if not ( x.typ in [bools,notyp]){*x类型不是布尔类型或无类型时*}
        then error(17);{*显示布尔类型错误*}
        lc1 := lc;
        emit(11);  { jmpc }
        if sy = thensy{*当前符号为then时*}
        then insymbol{*读下一符号*}
        else begin
               error(52);{*then符号错误*}
               if sy = dosy{*当前符号为do时*}
               then insymbol{*读下一符号*}
             end;
        statement( fsys+[elsesy]);{*分析处理各种语句*}
        if sy = elsesy{*当前符号为else时*}
        then begin
               insymbol;{*读下一符号*}
               lc2 := lc;
                emit(10);{*生成P代码指令代码长度*}
               code[lc1].y := lc;
               statement(fsys);{*分析处理各种语句*}
               code[lc2].y := lc
             end
        else code[lc1].y := lc
end { ifstatement };

    procedure casestatement;{*处理case语句*}
      var x : item;
          i,j,k,lc1 : integer;
      casetab : array[1..csmax]of
                     packed record
                       val,lc : index
                     end;
      exittab : array[1..csmax] of integer;

procedure caselabel;{*处理case语句中的标号，将各标号对应的目标代码入口地址填入casetab表中，并检查标号有无重复定义*}
        var lab : conrec;
        k : integer;
        begin
          constant( fsys+[comma,colon],lab );{*处理程序中出现的常量，并由参数（c）返回该常量的类型和数值*}
          if lab.tp <> x.typ{*若x类型和符号表登录的类型不一致，则显示case类型错误*}
          then error(47)
          else if i = csmax
               then fatal(6){*case表溢出*}
               else begin
                      i := i+1;
                      k := 0;
                      casetab[i].val := lab.i;{*登入case表信息*}
                      casetab[i].lc := lc;
                      repeat
                        k := k+1
                      until casetab[k].val = lab.i;
                      if k < i
                      then error(1); { multiple definition }{*重定义错误*}
                    end
        end { caselabel };

      procedure onecase;{*处理case语句中的一个分支*}
        begin
          if sy in constbegsys{*若当前符号是symbol集合中的元素*}
          then begin
                 caselabel;{*处理case语句中的标号，将各标号对应的目标代码入口地址填入casetab表中，并检查标号有无重复定义*}
                 while sy = comma do{*若当前符号为逗号*}
                   begin
                     insymbol;{*读下一符号*}
                     caselabel{*处理case语句中的标号，将各标号对应的目标代码入口地址填入casetab表中，并检查标号有无重复定义*}
                   end;
                 if sy = colon{*若当前符号为冒号*}
                 then insymbol{*读下一符号*}
                 else error(5);{*否则显示缺失冒号错误*}
                 statement([semicolon,endsy]+fsys);{*分析处理各种语句*}
                j := j+1;
                 exittab[j] := lc;
                  emit(10){*生成P代码指令代码长度*}
               end
          end { onecase };
      begin  { casestatement  }
        insymbol;{*读下一符号*}
        i := 0;
        j := 0;
        expression( fsys + [ofsy,comma,colon],x );{*分析处理表达式，由参数（x）返回求值*}
        if not( x.typ in [ints,bools,chars,notyp ]){*x类型不是整型或布尔类型或字符型或无类型时，显示case类型错误*}
        then error(23);
        lc1 := lc;
        emit(12); {jmpx}{*跳出过程*}
        if sy = ofsy{*当前符号为of时*}
        then insymbol{*读下一符号*}
        else error(8);{*否则显示缺失of错误*}
        onecase;{*处理case语句中的一个分支*}
        while sy = semicolon do
          begin
            insymbol;{*读下一符号*}
            onecase{*处理case语句中的一个分支*}
          end;
        code[lc1].y := lc;
        for k := 1 to i do
          begin
            emit1( 13,casetab[k].val);{*生成P代码指令行代码长度和位宽*}
            emit1( 13,casetab[k].lc);{*生成P代码指令行代码长度和位宽*}
          end;
        emit1(10,0);{*生成P代码指令行代码长度和位宽*}
        for k := 1 to j do
        code[exittab[k]].y := lc;
        if sy = endsy{*当前符号为end时*}
        then insymbol{*读下一符号*}
        else error(57){*否则显示end缺失错误*}
      end { casestatement };

    procedure repeatstatement;{*处理repeat语句*}
      var x : item;
          lc1: integer;
      begin
        lc1 := lc;
        insymbol;{*读下一符号*}
        statement( [semicolon,untilsy]+fsys);{*分析处理各种语句*}
        while sy in [semicolon]+statbegsys do
          begin
            if sy = semicolon{*当前符号为引号时*}
            then insymbol{*读下一符号*}
            else error(14);{*否则显示引号缺失错误*}
            statement([semicolon,untilsy]+fsys){*分析处理各种语句*}
          end;
        if sy = untilsy
        then begin
               insymbol;{*读下一符号*}
               expression(fsys,x);{*分析处理表达式，由参数（x）返回求值*}
               if not(x.typ in [bools,notyp] ){*x类型不是布尔类型或无类型时*}
               then error(17);{*显示布尔类型错误*}
               emit1(11,lc1);{*生成P代码指令行代码长度和位宽*}
             end
        else error(53){*显示until缺失错误*}
      end { repeatstatement };

    procedure whilestatement;{*处理while语句*}
      var x : item;
          lc1,lc2 : integer;
      begin
        insymbol;{*读下一符号*}
        lc1 := lc;
        expression( fsys+[dosy],x);{*分析处理表达式，由参数（x）返回求值*}
        if not( x.typ in [bools, notyp] ){*x类型不是布尔类型或无类型时*}
        then error(17);{*显示布尔类型错误*}
        lc2 := lc;
        emit(11);{*生成P代码指令代码长度*}
        if sy = dosy{*当前符号为do时*}
        then insymbol{*读下一符号*}
        else error(54);{*否则显示do缺失错误*}
        statement(fsys);{*分析处理各种语句*}
        emit1(10,lc1);{*生成P代码指令行代码长度和位宽*}
        code[lc2].y := lc
end { whilestatement };

    procedure forstatement;{*处理for语句*}
      var  cvt : types;
          x :  item;
          i,f,lc1,lc2 : integer;
        begin
        insymbol;{*读下一符号*}
        if sy = ident{*若当前符号为标识符*}
        then begin
               i := loc(id);{*找到标识符在符号表中的位置*}
               insymbol;{*读下一符号*}
               if i = 0
               then cvt := ints
               else if tab[i].obj = vvariable{*若tab[i]的种类为变量*}
                    then begin
                           cvt := tab[i].typ;{*记录tab[i]的类型*}
                           if not tab[i].normal{*如果ab[i]为变量形参*}
                           then error(37){*显示变量名错误*}
                            else emit2(0,tab[i].lev, tab[i].adr );
                            if not ( cvt in [notyp, ints, bools, chars]){*若cvt不是无类型或整型或布尔类型或字符型*}
                           then error(18){*显示变量类型错误*}
                         end
                else begin
                    error(37);{*变量名错误*}
                    cvt := ints
                end
             end
        else skip([becomes,tosy,downtosy,dosy]+fsys,2);{*跳读源程序，直至取来的符号属于给出的符号集为止，并打印出错标志*}
        if sy = becomes{*若当前符号为赋值符*}
        then begin
               insymbol;{*读下一符号*}
               expression( [tosy, downtosy,dosy]+fsys,x);{*分析处理表达式，由参数（x）返回求值*}
               if x.typ <> cvt{*如果x类型和cvt不同，显示类型错误*}
               then error(19);
             end
        else skip([tosy, downtosy,dosy]+fsys,51);{*跳读源程序，直至取来的符号属于给出的符号集为止，并打印出错标志*}
        f := 14;
        if sy in [tosy,downtosy]{*若当前符号为to或downto*}
        then begin
               if sy = downtosy{*若当前符号为downto*}
               then f := 16;
               insymbol;{*读下一符号*}
               expression([dosy]+fsys,x);{*分析处理表达式，由参数（x）返回求值*}
               if x.typ <> cvt{*如果x类型和cvt不同，显示类型错误*}
               then error(19)
             end
        else skip([dosy]+fsys,55);{*跳读源程序，直至取来的符号属于给出的符号集为止，并打印出错标志*}
        lc1 := lc;
        emit(f);{*生成P代码指令代码长度*}
        if sy = dosy{*若当前符号为do*}
        then insymbol{*读下一符号*}
        else error(54);{*否则显示do缺失错误*}
        lc2 := lc;
        statement(fsys);{*分析处理各种语句*}
        emit1(f+1,lc2);{*生成P代码指令行代码长度和位宽*}
        code[lc1].y := lc
end { forstatement };

    procedure standproc( n: integer );{*处理标准（输入/输出）过程调用*}
      var i,f : integer;
          x,y : item;
      begin
        case n of
          1,2 : begin { read }
                  if not iflag
                  then begin
                         error(20);{*程序参数错误*}
                         iflag := true
                       end;
                  if sy = lparent{*若当前符号为左括号*}
                  then begin
                         repeat
                           insymbol;{*读下一符号*}
                           if sy <> ident{*若当前符号不为标识符，显示识别错误*}
                           then error(2)
                           else begin
                                  i := loc(id);{*找到标识符在符号表中的位置*}
                                  insymbol;{*读下一符号*}
                                  if i <> 0
                                  then if tab[i].obj <> vvariable{*若tab[i]种类不是变量，显示变量名错误*}
                                       then error(37)
                                       else begin
                                              x.typ := tab[i].typ;{*记录tab[i]信息*}
                                              x.ref := tab[i].ref;
                                              if tab[i].normal
                                              then f := 0
                                              else f := 1;
                                              emit2(f,tab[i].lev,tab[i].adr);{*生成P代码指令代码长度、位宽和范围*}
                                              if sy in [lbrack,lparent,period]{*若当前符号为(或[或.*}
                                              then selector( fsys+[comma,rparent],x);{*处理结构变量：数组下标变量或记录成员变量*}
                                              if x.typ in [ints,reals,chars,notyp]{*若x类型为整型或实型或字符型或无类型*}                                              then emit1(27,ord(x.typ))
                                              else error(41){*显示类型错误*}
                                            end
                                end;
                           test([comma,rparent],fsys,6);{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
                         until sy <> comma;{*若当前符号不为逗号*}
                         if sy = rparent{*当前符号为右括号*}
                         then insymbol{*读下一符号*}
                         else error(4){*否则显示右括号缺失错误*}
                       end;
                  if n = 2
                  then emit(62){*生成P代码指令代码长度*}
                end;
          3,4 : begin { write }
                  if sy = lparent{*若当前符号为左括号*}
                  then begin
                         repeat
                           insymbol;{*读下一符号*}
                           if sy = stringcon{*若当前符号为字符串*}
                           then begin
                                  emit1(24,sleng);{*生成P代码指令行代码长度和位宽*}
                                  emit1(28,inum);{*生成P代码指令行代码长度和位宽*}
                                  insymbol{*读下一符号*}
                                end
                           else begin
                                  expression(fsys+[comma,colon,rparent],x);{*分析处理表达式，由参数（x）返回求值*}
                                  if not( x.typ in stantyps ){*若x类型不在stantyps集合中，显示类型错误*}
                                  then error(41);
                                  if sy = colon
                                  then begin
                                        insymbol;
                                         expression( fsys+[comma,colon,rparent],y);{*分析处理表达式，由参数（x）返回求值*}
                                          if y.typ <> ints{*若y类型不为整型，则显示整型错误*}
                                         then error(43);
                                         if sy = colon{*若当前符号为冒号*}
                                         then begin
                                                if x.typ <> reals{*若x类型不为实型，则显示实型错误*}
                                                then error(42);
                                                insymbol;{*读下一符号*}
                                                expression(fsys+[comma,rparent],y);{*分析处理表达式，由参数（x）返回求值*}
                                                if y.typ <> ints{*若y类型不为整型，则显示整型错误*}
                                                then error(43);
                                                emit(37){*生成P代码指令行代码长度*}
                                              end
                                         else emit1(30,ord(x.typ)){*生成P代码指令行代码长度和位宽*}
                                       end
                                  else emit1(29,ord(x.typ))
                          end
                         until sy <> comma;{*直至当前符号不为逗号*}
                         if sy = rparent{*若当前符号为右括号*}
                         then insymbol{*读下一符号*}
                         else error(4){*否则显示右括号缺失错误*}
                       end;
                  if n = 4
                  then emit(63){*生成P代码指令行代码长度*}
                end; { write }
        end { case };
      end { standproc } ;
    begin { statement }
      if sy in statbegsys+[ident]{*若当前符号在集合中*}
      then case sy of
             ident : begin{*若当前符号标识符*}
                       i := loc(id);{*找到标识符在符号表中的位置*}
                       insymbol;{*读下一符号*}
                       if i <> 0
                       then case tab[i].obj of
                              konstant,typel : error(45);{*变量过程错误*}
                              vvariable:       assignment( tab[i].lev,tab[i].adr);
                              prozedure:       if tab[i].lev <> 0{*若为过程，且嵌套层数不为0*}
                                               then call(fsys,i){*处理非标准的过程或函数调用*}
                                               else standproc(tab[i].adr);{*处理标准（输入/输出）过程调用*}
                              funktion:        if tab[i].ref = display[level]{*若为函数*}
                                               then assignment(tab[i].lev+1,0){*处理赋值语句*}
                                               else error(45){*变量过程错误*}
                            end { case }
                     end;
             beginsy : compoundstatement;{*处理复合语句*}
             ifsy    : ifstatement;{*处理if语句*}
             casesy  : casestatement;{*处理case语句*}
             whilesy : whilestatement;{*处理while语句*}
             repeatsy: repeatstatement;{*处理repeat语句*}
             forsy   : forstatement;{*处理for语句*}
           end;  { case }
      test( fsys, [],14);{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
    end { statement };
  begin  { block }
    dx := 5;
    prt := t;
    if level > lmax
    then fatal(5);{*若嵌套层数大于7，则嵌套层数溢出*}
    test([lparent,colon,semicolon],fsys,14);{*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
    enterblock;{*登录分程序表*}
    prb := b;
    display[level] := b;
    tab[prt].typ := notyp;
    tab[prt].ref := prb;
    if ( sy = lparent ) and ( level > 1 ){*若当前符号为左括号且嵌套层数大于1*}
    then parameterlist;{*处理过程或函数说明中的形参表，将形参及有关信息登录到符号表中*}
    btab[prb].lastpar := t;{*登入分程序表信息*}
    btab[prb].psize := dx;
    if isfun{*若为函数*}
    then if sy = colon{*若当前符号为冒号*}
         then begin
                insymbol; { function type }
                if sy = ident
                then begin
                       x := loc(id);{*找到标识符在符号表中的位置*}
                       insymbol;{*读下一符号*}
                       if x <> 0
                       then if tab[x].typ in stantyps{*若函数类型正确，则登录函数信息*}
                            then tab[prt].typ := tab[x].typ
                            else error(15){*否则显示函数类型错误*}
                     end
                else skip( [semicolon]+fsys,2 ){*跳读源程序，直至取来的符号属于给出的符号集为止，并打印出错标志*}
              end
         else error(5);{*冒号缺失错误*}
    if sy = semicolon{*若当前符号为分号*}
    then insymbol{*读下一符号*}
    else error(14);{*否则显示分号缺失错误*}
    repeat
      if sy = constsy{*若当前符号为 constsy，处理常量定义，将常量名及其相应信息填入符号表*}
      then constdec;
      if sy = typesy{*若当前符号为typesy，处理类型定义，并将类型名及其相应信息填入符号表*}
      then typedeclaration;
      if sy = varsy{*若当前符号为varsy，处理变量定义，将变量名及其相应信息填入符号表*}
      then variabledeclaration;
      btab[prb].vsize := dx;
      while sy in [procsy,funcsy] do{*若当前符号为过程名或函数名*}
        procdeclaration;{*处理过程或函数声明，将过程（函数）名填入符号表，递归调用block分析处理程序（层次：level+1）*}
      test([beginsy],blockbegsys+statbegsys,56){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
    until sy in statbegsys;
    tab[prt].adr := lc;{*记录*}
    insymbol;{*读下一符号*}
    statement([semicolon,endsy]+fsys);{*分析处理各种语句*}
    while sy in [semicolon]+statbegsys do
      begin
        if sy = semicolon{*若当前符号为分号*}
        then insymbol{*读下一符号*}
        else error(14);{*否则显示分号缺失错误*}
        statement([semicolon,endsy]+fsys);{*分析处理各种语句*}
      end;
    if sy = endsy{*若当前符号为end*}
    then insymbol{*读下一符号*}
    else error(57);{*否则显示end缺失错误*}
    test( fsys+[period],[],6 ){*测试当前符号是否合法，若不合法，打印出错标志并跳读*}
  end { block };



procedure interpret;{*P代码解释执行程序*}
  var ir : order ;         { instruction buffer }{*指令缓冲区*}
      pc : integer;        { program counter }{*程序计数器*}
      t  : integer;        { top stack index }{*栈顶指针*}
b  : integer;        { base index }
      h1,h2,h3: integer;
      lncnt,ocnt,blkcnt,chrcnt: integer;     { counters }
      ps : ( run,fin,caschk,divchk,inxchk,stkchk,linchk,lngchk,redchk );
fld: array [1..4] of integer;  { default field widths }{*默认字段宽度*}
      display : array[0..lmax] of integer;
      s  : array[1..stacksize] of   { blockmark:     }{*标记块*}
            record
              case cn : types of        { s[b+0] = fct result }{*函数结果*}
                ints : (i: integer );   { s[b+1] = return adr }{*返回地址*}
                reals :(r: real );      { s[b+2] = static link }{*静态连接*}
                bools :(b: boolean );   { s[b+3] = dynamic link }{*动态链接*}
                chars :(c: char )       { s[b+4] = table index }{*索引表*}
end;

  procedure dump;{*程序运行时，卸出打印现场剖析信息（display，t、b及运行栈S的内容）*}
    var p,h3 : integer;
begin
      h3 := tab[h2].lev;
      writeln(psout);
      writeln(psout);
      writeln(psout,'       calling ', tab[h2].name );
      writeln(psout,'         level ',h3:4);
      writeln(psout,' start of code ',pc:4);
      writeln(psout);
      writeln(psout);
      writeln(psout,' contents of display ');
      writeln(psout);
      for p := h3 downto 0 do
        writeln(psout,p:4,display[p]:6);{*卸出打印display内容*}
      writeln(psout);
      writeln(psout);
      writeln(psout,' top of stack  ',t:4,' frame base ':14,b:4);{*卸出打印t、b的内容*}
      writeln(psout);
      writeln(psout);
      writeln(psout,' stack contents ':20);{*卸出打印运行栈S内容*}
      writeln(psout);
      for p := t downto 1 do
        writeln( psout, p:14, s[p].i:8);
      writeln(psout,'< = = = >':22)
    end; {dump }

  procedure inter0;{*对值操作符PCODE所对应的操作*}
    begin
      case ir.f of
        0 : begin { load addrss }{*第f条代码PCODE为0，加载地址*}
              t := t + 1;
              if t > stacksize
              then ps := stkchk
              else s[t].i := display[ir.x]+ir.y
            end;
        1 : begin  { load value }{*第f条代码PCODE为1，加载变量*}
              t := t + 1;
              if t > stacksize
              then ps := stkchk
              else s[t] := s[display[ir.x]+ir.y]
            end;
        2 : begin  { load indirect }{*第f条代码PCODE为2，间接加载*}
              t := t + 1;
              if t > stacksize
              then ps := stkchk
              else s[t] := s[s[display[ir.x]+ir.y].i]
            end;
        3 : begin  { update display }{*第f条代码PCODE为3，更新display*}
              h1 := ir.y;
              h2 := ir.x;
              h3 := b;
              repeat
                display[h1] := h3;
                h1 := h1-1;
                h3 := s[h3+2].i
              until h1 = h2
            end;
        8 : case ir.y of{**}{*第f条代码为8，转而看第y条代码PCODE*}
              0 : s[t].i := abs(s[t].i);
              1 : s[t].r := abs(s[t].r);
              2 : s[t].i := sqr(s[t].i);
              3 : s[t].r := sqr(s[t].r);
              4 : s[t].b := odd(s[t].i);
              5 : s[t].c := chr(s[t].i);
              6 : s[t].i := ord(s[t].c);
              7 : s[t].c := succ(s[t].c);
              8 : s[t].c := pred(s[t].c);
              9 : s[t].i := round(s[t].r);
              10 : s[t].i := trunc(s[t].r);
              11 : s[t].r := sin(s[t].r);
              12 : s[t].r := cos(s[t].r);
              13 : s[t].r := exp(s[t].r);
              14 : s[t].r := ln(s[t].r);
              15 : s[t].r := sqrt(s[t].r);
              16 : s[t].r := arcTan(s[t].r);
              17 : begin
                     t := t+1;
                     if t > stacksize
                     then ps := stkchk
                     else s[t].b := eof(prd)
                   end;
              18 : begin
                     t := t+1;
                     if t > stacksize
                     then ps := stkchk
                     else s[t].b := eoln(prd)
                   end;
            end;
        9 : s[t].i := s[t].i + ir.y; { offset }
      end { case ir.y }
    end; { inter0 }

procedure inter1;{*对跳转类PCODE所对应的操作*}
    var h3, h4: integer;
    begin
      case ir.f of
        10 : pc := ir.y ; { jump }{*跳转至第y条指令*}
        11 : begin  { conditional jump }{*有条件跳转*}
               if not s[t].b
                then pc := ir.y;
               t := t - 1
              end;
        12 : begin { switch }
               h1 := s[t].i;
               t := t-1;
               h2 := ir.y;
               h3 := 0;
               repeat
                 if code[h2].f <> 13
                 then begin
                        h3 := 1;
                        ps := caschk
                      end
                 else if code[h2].y = h1
                      then begin
                             h3 := 1;
                             pc := code[h2+1].y
                           end
                      else h2 := h2 + 2
               until h3 <> 0
             end;
        14 : begin { for1up }{*for++初始判断*}
               h1 := s[t-1].i;
               if h1 <= s[t].i
               then s[s[t-2].i].i := h1
               else begin
                      t := t - 3;
                      pc := ir.y
                    end
             end;
        15 : begin { for2up }{*for++结束判断*}
               h2 := s[t-2].i;
               h1 := s[h2].i+1;
               if h1 <= s[t].i
               then begin
                      s[h2].i := h1;
                      pc := ir.y
                    end
               else t := t-3;
             end;
        16 : begin  { for1down }{*for--初始判断*}
               h1 := s[t-1].i;
               if h1 >= s[t].i
               then s[s[t-2].i].i := h1
               else begin
                      pc := ir.y;
                      t := t - 3
                    end
             end;
        17 : begin  { for2down }{*for--结束判断*}
               h2 := s[t-2].i;
               h1 := s[h2].i-1;
               if h1 >= s[t].i
               then begin
                      s[h2].i := h1;
                      pc := ir.y
                    end
               else t := t-3;
             end;
        18 : begin  { mark stack }{*标记堆栈*}
               h1 := btab[tab[ir.y].ref].vsize;
               if t+h1 > stacksize
               then ps := stkchk
               else begin
                      t := t+5;
                      s[t-1].i := h1-1;
                      s[t].i := ir.y
                    end
             end;
        19 : begin  { call }{*过程或函数调用*}
               h1 := t-ir.y;  { h1 points to base }{*h1指向基地址*}
               h2 := s[h1+4].i;  { h2 points to tab }{*h2指向符号表符号初地址*}
               h3 := tab[h2].lev;{*h3为tab[h2]过程符号嵌套层次*}
               display[h3+1] := h1;
               h4 := s[h1+3].i+h1;
               s[h1+1].i := pc;
               s[h1+2].i := display[h3];
               s[h1+3].i := b;
               for h3 := t+1 to h4 do
                 s[h3].i := 0;
               b := h1;
               t := h4;
               pc := tab[h2].adr;
               if stackdump
               then dump
             end;
      end { case }
    end; { inter1 }

  procedure inter2;{*对加载类PCODE所对应的操作*}
    begin
      case ir.f of
        20 : begin   { index1 }
               h1 := ir.y;  { h1 points to atab }{*h1指向数组信息向量表*}
               h2 := atab[h1].low;
               h3 := s[t].i;
               if h3 < h2
               then ps := inxchk
               else if h3 > atab[h1].high
                    then ps := inxchk
                    else begin
                           t := t-1;
                           s[t].i := s[t].i+(h3-h2)
                         end
             end;
        21 : begin  { index }
               h1 := ir.y ; { h1 points to atab }{*h1指向数组信息向量表*}
               h2 := atab[h1].low;
               h3 := s[t].i;
               if h3 < h2
               then ps := inxchk
               else if h3 > atab[h1].high
                    then ps := inxchk
                    else begin
                           t := t-1;
                           s[t].i := s[t].i + (h3-h2)*atab[h1].elsize
                         end
             end;
        22 : begin  { load block }{*加载分程序*}
               h1 := s[t].i;
               t := t-1;
               h2 := ir.y+t;
               if h2 > stacksize
               then ps := stkchk
               else while t < h2 do
                      begin
                        t := t+1;
                        s[t] := s[h1];
                        h1 := h1+1
                      end
             end;
        23 : begin  { copy block }{*复制分程序*}
               h1 := s[t-1].i;
               h2 := s[t].i;
               h3 := h1+ir.y;
               while h1 < h3 do
                 begin
                   s[h1] := s[h2];
                   h1 := h1+1;
                   h2 := h2+1
                 end;
               t := t-2
             end;
        24 : begin  { literal }
               t := t+1;
               if t > stacksize
               then ps := stkchk
               else s[t].i := ir.y
             end;
        25 : begin  { load real }{*加载实数*}
               t := t+1;
               if t > stacksize
               then ps := stkchk
               else s[t].r := rconst[ir.y]
             end;
        26 : begin  { float }{*加载浮点数*}
               h1 := t-ir.y;
               s[h1].r := s[h1].i
             end;
        27 : begin  { read }{*读操作*}
               if eof(prd)
               then ps := redchk
               else case ir.y of
                      1 : read(prd, s[s[t].i].i);
                      2 : read(prd, s[s[t].i].r);
                      4 : read(prd, s[s[t].i].c);
                    end;
               t := t-1
             end;
        28 : begin   { write string }{*写字符串*}
                h1 := s[t].i;
               h2 := ir.y;
               t := t-1;
               chrcnt := chrcnt+h1;
                if chrcnt > lineleng
               then ps := lngchk;
               repeat
                 write(prr,stab[h2]);
                 h1 := h1-1;
                 h2 := h2+1
               until h1 = 0
             end;
        29 : begin  { write1 }
               chrcnt := chrcnt + fld[ir.y];
               if chrcnt > lineleng
               then ps := lngchk
               else case ir.y of
                      1 : write(prr,s[t].i:fld[1]);
                      2 : write(prr,s[t].r:fld[2]);
                      3 : if s[t].b
                          then write('true')
                          else write('false');
                      4 : write(prr,chr(s[t].i));
                    end;
               t := t-1
             end;
      end { case }
    end; { inter2 }

  procedure inter3;
    begin
      case ir.f of
        30 : begin { write2 }
               chrcnt := chrcnt+s[t].i;
               if chrcnt > lineleng
               then ps := lngchk
               else case ir.y of
                      1 : write(prr,s[t-1].i:s[t].i);
                      2 : write(prr,s[t-1].r:s[t].i);
                      3 : if s[t-1].b
                          then write('true')
                          else write('false');
                    end;
               t := t-2
             end;
        31 : ps := fin;
        32 : begin  { exit procedure }{*退出过程*}
               t := b-1;
               pc := s[b+1].i;
               b := s[b+3].i
             end;
        33 : begin  { exit function }{*退出函数*}
               t := b;
               pc := s[b+1].i;
               b := s[b+3].i
             end;
        34 : s[t] := s[s[t].i];
        35 : s[t].b := not s[t].b;
        36 : s[t].i := -s[t].i;
        37 : begin
               chrcnt := chrcnt + s[t-1].i;
               if chrcnt > lineleng
               then ps := lngchk
               else write(prr,s[t-2].r:s[t-1].i:s[t].i);
               t := t-3
             end;
        38 : begin  { store }{*存储*}
               s[s[t-1].i] := s[t];
               t := t-2
             end;
        39 : begin
               t := t-1;
               s[t].b := s[t].r=s[t+1].r
             end;
      end { case }
    end; { inter3 }

  procedure inter4;
    begin
      case ir.f of
        40 : begin
               t := t-1;
               s[t].b := s[t].r <> s[t+1].r
             end;
        41 : begin
               t := t-1;
               s[t].b := s[t].r < s[t+1].r
             end;
        42 : begin
               t := t-1;
               s[t].b := s[t].r <= s[t+1].r
             end;
        43 : begin
               t := t-1;
               s[t].b := s[t].r > s[t+1].r
             end;
        44 : begin
               t := t-1;
               s[t].b := s[t].r >= s[t+1].r
             end;
        45 : begin
               t := t-1;
               s[t].b := s[t].i = s[t+1].i
             end;
        46 : begin
               t := t-1;
               s[t].b := s[t].i <> s[t+1].i
             end;
        47 : begin
               t := t-1;
               s[t].b := s[t].i < s[t+1].i
             end;
        48 : begin
               t := t-1;
               s[t].b := s[t].i <= s[t+1].i
             end;
        49 : begin
               t := t-1;
               s[t].b := s[t].i > s[t+1].i
             end;
      end { case }
    end; { inter4 }

  procedure inter5;
    begin
      case ir.f of
        50 : begin
               t := t-1;
               s[t].b := s[t].i >= s[t+1].i
             end;
        51 : begin
               t := t-1;
               s[t].b := s[t].b or s[t+1].b
             end;
        52 : begin
               t := t-1;
               s[t].i := s[t].i+s[t+1].i
             end;
        53 : begin
               t := t-1;
               s[t].i := s[t].i-s[t+1].i
             end;
        54 : begin
               t := t-1;
               s[t].r := s[t].r+s[t+1].r;
             end;
        55 : begin
               t := t-1;
               s[t].r := s[t].r-s[t+1].r;
             end;
        56 : begin
               t := t-1;
               s[t].b := s[t].b and s[t+1].b
             end;
        57 : begin
               t := t-1;
               s[t].i := s[t].i*s[t+1].i
             end;
        58 : begin
               t := t-1;
               if s[t+1].i = 0
               then ps := divchk
               else s[t].i := s[t].i div s[t+1].i
             end;
        59 : begin
               t := t-1;
               if s[t+1].i = 0
               then ps := divchk
               else s[t].i := s[t].i mod s[t+1].i
             end;
      end { case }
    end; { inter5 }

  procedure inter6;
    begin
      case ir.f of
        60 : begin
               t := t-1;
               s[t].r := s[t].r*s[t+1].r;
             end;
        61 : begin
               t := t-1;
               s[t].r := s[t].r/s[t+1].r;
             end;
        62 : if eof(prd)
             then ps := redchk
             else readln;
        63 : begin
               writeln(prr);
               lncnt := lncnt+1;
               chrcnt := 0;
               if lncnt > linelimit
               then ps := linchk
             end
      end { case };
    end; { inter6 }
  begin { interpret }
    s[1].i := 0;
    s[2].i := 0;
    s[3].i := -1;
    s[4].i := btab[1].last;
    display[0] := 0;
    display[1] := 0;
    t := btab[2].vsize-1;
    b := 0;
    pc := tab[s[4].i].adr;
    lncnt := 0;
    ocnt := 0;
    chrcnt := 0;
    ps := run;
    fld[1] := 10;
    fld[2] := 22;
    fld[3] := 10;
    fld[4] := 1;
    repeat{*读PCODE直至结束*}
      ir := code[pc];
      pc := pc+1;
      ocnt := ocnt+1;
      case ir.f div 10 of
0 : inter0;
        1 : inter1;
        2 : inter2;
        3 : inter3;
        4 : inter4;
5 : inter5;
        6 : inter6;
      end; { case }
    until ps <> run;

    if ps <> fin{*如果ps和fin不等，表示编译错误*}
    then begin
           writeln(prr);
           write(prr, ' halt at', pc :5, ' because of ');
           case ps of{*各种报错信息*}
             caschk  : writeln(prr,'undefined case');
             divchk  : writeln(prr,'division by 0');
             inxchk  : writeln(prr,'invalid index');
             stkchk  : writeln(prr,'storage overflow');
             linchk  : writeln(prr,'too much output');
             lngchk  : writeln(prr,'line too long');
             redchk  : writeln(prr,'reading past end or file');
           end;
           h1 := b;
           blkcnt := 10;    { post mortem dump }{*事后储存*}
           repeat
             writeln( prr );
             blkcnt := blkcnt-1;
             if blkcnt = 0
             then h1 := 0;
             h2 := s[h1+4].i;
             if h1 <> 0
             then writeln( prr, '',tab[h2].name, 'called at', s[h1+1].i:5);
             h2 := btab[tab[h2].ref].last;
             while h2 <> 0 do
               with tab[h2] do
                 begin
                   if obj = vvariable
                   then if typ in stantyps
                        then begin
                               write(prr,'',name,'=');
                               if normal
                               then h3 := h1+adr
                               else h3 := s[h1+adr].i;
                               case typ of
                                 ints : writeln(prr,s[h3].i);
                                 reals: writeln(prr,s[h3].r);
                                 bools: if s[h3].b
                                        then writeln(prr,'true')
                                        else writeln(prr,'false');
                                 chars: writeln(prr,chr(s[h3].i mod 64 ))
                               end
                             end;
                   h2 := link
                 end;
             h1 := s[h1+3].i
           until h1 < 0
         end;
    writeln(prr);
    writeln(prr,ocnt,' steps');
  end; { interpret }



procedure setup;{*建立保留字表（key）和特定字符表（sps）*}
  begin
    key[1] := 'and       ';
    key[2] := 'array     ';
    key[3] := 'begin     ';
    key[4] := 'case      ';
    key[5] := 'const     ';
    key[6] := 'div       ';
    key[7] := 'do        ';
    key[8] := 'downto    ';
    key[9] := 'else      ';
    key[10] := 'end       ';
    key[11] := 'for       ';
    key[12] := 'function  ';
    key[13] := 'if        ';
    key[14] := 'mod       ';
    key[15] := 'not       ';
    key[16] := 'of        ';
    key[17] := 'or        ';
    key[18] := 'procedure ';
    key[19] := 'program   ';
    key[20] := 'record    ';
    key[21] := 'repeat    ';
    key[22] := 'then      ';
    key[23] := 'to        ';
    key[24] := 'type      ';
    key[25] := 'until     ';
    key[26] := 'var       ';
    key[27] := 'while     ';

    ksy[1] := andsy;
    ksy[2] := arraysy;
    ksy[3] := beginsy;
    ksy[4] := casesy;
    ksy[5] := constsy;
    ksy[6] := idiv;
    ksy[7] := dosy;
    ksy[8] := downtosy;
    ksy[9] := elsesy;
    ksy[10] := endsy;
    ksy[11] := forsy;
    ksy[12] := funcsy;
    ksy[13] := ifsy;
    ksy[14] := imod;
    ksy[15] := notsy;
    ksy[16] := ofsy;
    ksy[17] := orsy;
    ksy[18] := procsy;
    ksy[19] := programsy;
    ksy[20] := recordsy;
    ksy[21] := repeatsy;
    ksy[22] := thensy;
    ksy[23] := tosy;
    ksy[24] := typesy;
    ksy[25] := untilsy;
    ksy[26] := varsy;
    ksy[27] := whilesy;


    sps['+'] := plus;
    sps['-'] := minus;
    sps['*'] := times;
    sps['/'] := rdiv;
    sps['('] := lparent;
    sps[')'] := rparent;
    sps['='] := eql;
    sps[','] := comma;
    sps['['] := lbrack;
    sps[']'] := rbrack;
    sps[''''] := neq;
    sps['!'] := andsy;
    sps[';'] := semicolon;
  end { setup };

procedure enterids;{*在符号表中登录标准的类型（基本类型）、函数和过程的名字，以及他们的相应信息*}
  begin
    enter('          ',vvariable,notyp,0); { sentinel }
    enter('false     ',konstant,bools,0);
    enter('true      ',konstant,bools,1);
    enter('real      ',typel,reals,1);
    enter('char      ',typel,chars,1);
    enter('boolean   ',typel,bools,1);
    enter('integer   ',typel,ints,1);
    enter('abs       ',funktion,reals,0);
    enter('sqr       ',funktion,reals,2);
    enter('odd       ',funktion,bools,4);
    enter('chr       ',funktion,chars,5);
    enter('ord       ',funktion,ints,6);
    enter('succ      ',funktion,chars,7);
    enter('pred      ',funktion,chars,8);
    enter('round     ',funktion,ints,9);
    enter('trunc     ',funktion,ints,10);
    enter('sin       ',funktion,reals,11);
    enter('cos       ',funktion,reals,12);
    enter('exp       ',funktion,reals,13);
    enter('ln        ',funktion,reals,14);
    enter('sqrt      ',funktion,reals,15);
    enter('arctan    ',funktion,reals,16);
    enter('eof       ',funktion,bools,17);
    enter('eoln      ',funktion,bools,18);
    enter('read      ',prozedure,notyp,1);
    enter('readln    ',prozedure,notyp,2);
    enter('write     ',prozedure,notyp,3);
    enter('writeln   ',prozedure,notyp,4);
    enter('          ',prozedure,notyp,0);
  end;


begin  { main }      {*main函数*}
setup;{*初始化*}
  constbegsys := [ plus, minus, intcon, realcon, charcon, ident ];{*常量开始符号集合*}
  typebegsys := [ ident, arraysy, recordsy ];{*类型开始符号集合*}
  blockbegsys := [ constsy, typesy, varsy, procsy, funcsy, beginsy ];{*分程序开始符号集合*}
  facbegsys := [ intcon, realcon, charcon, ident, lparent, notsy ];{*函数开始符号集合*}
  statbegsys := [ beginsy, ifsy, whilesy, repeatsy, forsy, casesy ];{*语句开始符号集合*}
  stantyps := [ notyp, ints, reals, bools, chars ];{*标准类型结合*}
  lc := 0;{*初始化pc*}
  ll := 0;{*初始化当前行长度*}
  cc := 0;{*初始化程序计数器*}
  ch := ' ';{*初始化字符*}
  errpos := 0;
  errs := [];
  writeln( 'NOTE input/output for users program is console : ' );
  writeln;
  write( 'Source input file ?');{*要求输入代码输入文件*}
  readln( inf );
  assign( psin, inf );
  reset( psin );
  write( 'Source listing file ?');{*要求输入代码输出文件*}
  readln( outf );
  assign( psout, outf );
  rewrite( psout );
  assign ( prd, 'con' );
  write( 'result file : ' );{*要求输入结果输出文件*}
  readln( fprr );
  assign( prr, fprr );
  reset ( prd );
  rewrite( prr );

  t := -1;
  a := 0;
  b := 1;
  sx := 0;
  c2 := 0;
  display[0] := 1;
  iflag := false;
  oflag := false;
  skipflag := false;
  prtables := false;
  stackdump := false;

  insymbol;{*读下一符号*}

  if sy <> programsy{*如果当前符号不是program关键字，则报错*}
  then error(3)
  else begin
         insymbol;{*读下一符号*}
         if sy <> ident{*如果当前符号不是标识符，报错*}
         then error(2)
         else begin
                progname := id;
                insymbol;
                if sy <> lparent{*当前符号不是左括号，则报错*}
                then error(9)
                else repeat
                       insymbol;{*读下一符号*}
                       if sy <> ident{*如果当前符号不是标识符，报错*}
                       then error(2)
                       else begin
                              if id = 'input     '
                              then iflag := true
                              else if id = 'output    '
                                   then oflag := true
                                   else error(0);
                              insymbol{*读下一符号*}
                            end
                     until sy <> comma;{*读到逗号则停止*}
                if sy = rparent
                then insymbol
                else error(4);
                if not oflag then error(20)
              end
       end;
  enterids;{*在符号表中登录标准的类型（基本类型）、函数和过程的名字，以及他们的相应信息*}
  with btab[1] do{*初始化btab*}
    begin
      last := t;
      lastpar := 1;
      psize := 0;
      vsize := 0;
    end;
  block( blockbegsys + statbegsys, false, 1 );{*开始编译*}
  if sy <> period{*如果当前符号不是句号，则报错*}
  then error(2);
  emit(31);  { halt }
  if prtables
  then printtables;{*打印编译生成的符号表、分程序表、实常数表以及P代码*}
  if errs = []
  then interpret{*如果没有编译错误，执行P代码解释执行程序*}
  else begin
         writeln( psout );
         writeln( psout, 'compiled with errors' );
         writeln( psout );
         errormsg;{*否则打印编译错误*}
       end;
  writeln( psout );
  close( psout );{*关闭打开的代码输出文件*}
  close( prr ){*关闭打开的结果输出文件*}
end.                                              

