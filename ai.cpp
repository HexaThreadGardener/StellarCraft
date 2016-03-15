#include"teamstyle17.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
/**************V1.1.0***********************/
/*	�ײ��װ�뷽�� BY Zero Weight 2016/3/15 11:30	*/
/*	���еײ������ARΪ��ͷ����ע�������ظ�		*/
/*	�ṹ���װ��ʽ�μ�ʹ���ֲ�								*/
/*	�ٶ�˫������ֻ����һ����ֻ����������		*/
/*	���ϵ��ߣ�E(nergy)����Դ��
	A(dvancedEnergy����֮�����D(evour)������		*/
/*********************************************/





/******************�ײ����*******************/
//�㷨ִ���оݣ�������¼��ǰ�غ�����ͬ������	������AR_GO=GetTime����	ͬ��ʱ��
int AR_GO=-1;
//�����������������ṩ������������     ������AR_my.pos�� �õ�����λ��
PlayerObject AR_my;	
//�Է��������������ṩ�Է���������     ������AR_opposite.pos�� �õ��Է�λ��
Object AR_opposite;	
//�������죬һ�㲻���õ�������ѡ��������ڵײ����
int AR_MYTeam;		
//�Է��Ƿ�����Ұ�ڣ�ֻ��Ϊtrue��ʱ�� AR_opposite��������
bool AR_OppositeIN;		
//boss���������ṩboss��������     ������AR_boss.pos �õ�bossλ��
Object AR_boss;					
//boss�Ƿ�����Ұ�ڣ�ֻ��Ϊtrue��ʱ�� AR_boss��������
bool AR_BossIN;			
//��ʼֵΪ�㣬������¼��Ұ�ڵ�E��A��D     �����������ṩѭ����
int AR_NUME,AR_NUMA,AR_NUMD;	
//����E,A,D�������飬���ڵײ�Ե�ͼ�������	������AR_E[0].pos	�õ�һ������Դλ��
Object AR_E[kMaxObjectNumber],	AR_A[kMaxObjectNumber],AR_D[kMaxObjectNumber];
//��ͼ��һ�㲻���õ����Ѿ��ڵײ�Ե�ͼ���׽���	������AR_map->objectnumber �õ���������
const Map* AR_map;
//���������������޷����ƶ���һ�����÷���Ϊ��ͼ����
const Position AR_Center={kMapSize/2,kMapSize/2,kMapSize/2};
//
Position AR_speed = AR_Center;
/******************��ʼ������*******************/
//��ʼ��������������AIMain�ʼ����
void initialize(){		
	AR_BossIN=AR_OppositeIN=AR_NUMA=AR_NUMD=AR_NUME=0;
	AR_map=GetMap();
	AR_my=GetStatus()->objects[0];
	AR_MYTeam=GetStatus()->team_id;
	for(int i=0;i<AR_map->objects_number;i++){
		switch(AR_map->objects[i].type){
		case BOSS:{
			AR_BossIN=1;
			AR_boss=AR_map->objects[i];
			break;
				  }
		case ENERGY:{
			AR_E[AR_NUME]=AR_map->objects[i];
			AR_NUME++;
			break;
					}
		case ADVANCED_ENERGY:{
			AR_A[AR_NUMA]=AR_map->objects[i];
			AR_NUMA++;
			break;
					}
		case DEVOUR:{
			AR_D[AR_NUMD]=AR_map->objects[i];
			AR_NUMD++;
			break;
					}
		default:{
			if(AR_map->objects[i].team_id!=AR_MYTeam&&AR_map->objects[i].team_id!=-2) {
				AR_OppositeIN=1;
				AR_opposite=AR_map->objects[i];
			}
				}
		}
	}
}
void wait() {
	int i;
	for (i = 0;i < 10000000000;i++);
}




/******************��������*******************/
//��һ��������ģ��     ������length(A)==||A||;
double length(Position a){							
	return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}
//����������֮��ķ�������    ������PointTo(A,B)==AB;��A��B
Position PointTo(Position src,Position des){
	Position temp={des.x-src.x,des.y-src.y,des.z-src.z};
	return(temp);
}
//��������֮��ľ���     ������distance(A,B)==||AB||;
double distance(Position src,Position des){
	return length(PointTo(src,des));
}
//�Ƚ��������Ƿ����,���Ϊ0.001
bool equal(Position A, Position B) {
	return distance(A, B) < 0.001;
}
//��������   ������multiple(k,A)==kA;
Position multiple(double times,Position a){
	Position temp={a.x*times,a.y*times,a.z*times};
	return temp;
}





/******************�˶�����*******************/
//������ٶ���ָ��Ŀ���ƶ�     ������MoveTo(A);������ٶ���A�ƶ�
//��װʱ��֤��AΪ��ǰʸ������-1����ִ���κβ��������򷵻�0
int MoveTo(Position des){
	if ((!equal(des,AR_my.pos))&&(!equal(des,AR_speed))) {
		Move(AR_my.id, multiple(kMaxMoveSpeed / distance(AR_my.pos, des), PointTo(AR_my.pos, des)));
		AR_speed = des;
		return 0;
	}
	else return -1;
}





/******************�����Ƽ�(��������*******************/
//�����Ƽ����������ԣ����ɹ���������0��ʧ�ܲ�ִ�У�����-1
//���ܼ��ֲᣬ��������HEALTH
//�ڼ��ܵ����ļ����ϴ���
int cost(SkillType skill) {
	if (AR_my.skill_level[skill]) {
		return (kBasicSkillPrice[skill] << AR_my.skill_level[skill]);
	}
	else {
		int i, count;
		for (i = 0, count = 0;i < kSkillTypes;i++) {
			if (AR_my.skill_level[i]) count++;
		}
		return(kBasicSkillPrice[skill] << count);
	}
}
int update(SkillType skill) {
	if (AR_my.ability >= cost(skill))
	{
		UpgradeSkill(AR_my.id, skill);
		return 0;
	}
	else return -1;
}



/******************����ʹ��*******************/
//���¼���ʹ�����ע����ʹ���Ƿ�ɹ���0Ϊ�ɹ���-1ʧ��
int Lattack(Object aim) {
	if (AR_my.skill_level[LONG_ATTACK] && AR_my.skill_cd[LONG_ATTACK] == 0
		&&distance(AR_my.pos,aim.pos)<kLongAttackRange[AR_my.skill_level[LONG_ATTACK]] ){
		LongAttack(AR_my.id, aim.id);
		printf("LONG ATTACK AT\t%d\tLEVEL\t%d\n", GetTime(), AR_my.skill_level[LONG_ATTACK]);
		return 0;
	}
	else return -1;
}
int Sattack() {
	if (AR_my.skill_level[SHORT_ATTACK] && (AR_my.skill_cd[SHORT_ATTACK] == 0)) {
		ShortAttack(AR_my.id);
		printf("SHORT ATTACK AT\t%d\tLEVEL\t%d\n", GetTime(), AR_my.skill_level[SHORT_ATTACK]);
		return 0;
	}
	else {
		return -1;
	}
}
int shield() {
	if (AR_my.skill_level[SHIELD] && AR_my.skill_cd[SHIELD] == 0) {
		Shield(AR_my.id);
		printf("SHIELD AT\t%d\tLEVEL\t%d\n", GetTime(), AR_my.skill_level[SHIELD]);
		return 0;
	}
	else return -1;
}
int dash() {
	if (AR_my.skill_level[DASH] && AR_my.skill_cd[DASH] == 0) {
		Dash(AR_my.id);
		printf("DASH AT\t\t%d\tLEVEL\t%d\n", GetTime(), AR_my.skill_level[DASH]);
		return 0;
	}
	else return -1;
}
/**********�ײ��װ����**********/
//���ĺ���
void AIMain(){
	initialize();
	update(SHIELD);
	update(SHORT_ATTACK);
	update(DASH);
	dash();
	Sattack();
	shield();
	if (AR_BossIN&&AR_my.radius > 1.5*AR_boss.radius) MoveTo(AR_boss.pos);
	if(AR_NUMA)MoveTo(AR_A[0].pos);
	else if(AR_NUME)MoveTo(AR_E[0].pos);
	else MoveTo(AR_Center);
}
