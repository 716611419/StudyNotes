	area first, code, readonly
	code32
	entry
start
	; mov mvn 
	; ���ݴ���ָ��Ļ���Ӧ��
	mov r0, #1
	mov r1, #2

	; 1. add 
	add r2, r0, r1	; r2 = r0 + r1
	add r2, r0, #4
	add r2, r0, r1, lsl #2 ;  r2 = r0�������ң��������������ң��������ң�����������

 	mov r0, #0
	mov r1, #0xffffffff
  	mov r2, #0
	mov r3, #0x1
	; 2. adc  64λ�ӷ� r0, r1 =  r0, r1 + r2, r3����
	adds r1, r1, r3 ; r1 = r1 + r3	 �����S��׺
	adc r0, r0, r2	; r0 = r0 + r2 + c

	; 3. sub  rd = rn - op2 
	mov r0, #1
	sub r0, r0, #1	; r0 = r0 - 1

	; 4. sbc  64λ���� r0, r1 =  r0, r1 - r2, r3��
    ; cpsr c ���ڼӷ����� C = 1 ������н�λ�� C = 0 �޽�λ
	;		 ���ڼ������� C = 1 ������޽�λ�� C = 0 �н�λ
 	mov r0, #0
	mov r1, #0x0
  	mov r2, #0
	mov r3, #0x1
	subs r1, r1, r3 
	sbc r0, r0, r2

	; 5. rsb   subָ��  rd = rn - op2 	rsb = op2 - rn
	; eg �� R1 �Ĵ����ĸ���
	mov r0, #0
	sub r1, r0, r1
	; rsb ��ΪSUBָ��Ĳ��䣬RSB ʹ�� OP2��Ϊ�������������
	rsb r1, r1, #0 ; r1 = 0 - r1

	; 6... rsc	����λ�����������Ч��ͬ SBCָ��

	; 7. and 
	and r0, r0, #0x01       ; ȡ���Ϊ��ֵ
	and r0, r0, #0xfffffffe ; �����λ����

	; 8. orr
	orr r0, r0, #0x01       ; �����λ����1 

	; 9. eor  
	 
	; 10. bic λ��� 
	mov r0, #0xffffffff
	bic r0, r0, #0xff ; and r0, r0, #0xffffff00 




stop 
	b stop 
	end