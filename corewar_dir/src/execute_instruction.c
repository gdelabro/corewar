/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_instruction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-moul <lde-moul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/04 15:22:51 by lde-moul          #+#    #+#             */
/*   Updated: 2017/10/17 17:02:50 by lde-moul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

// !!!
// static char *sub_ocp(char sub_ocp)
// {
// 	switch (sub_ocp)
// 	{
// 		case 0: return "none"; break ;
// 		case REG_CODE: return "register"; break ;
// 		case DIR_CODE: return "direct"; break ;
// 		case IND_CODE: return "index"; break ;
// 		default: return "invalid"; break ;
// 	}
// }

// !!!
// static void print_ocp(char ocp)
// {
// 	printf("ocp: %s, ", sub_ocp((ocp >> 6) & 3));
// 	printf("ocp: %s, ", sub_ocp((ocp >> 4) & 3));
// 	printf("ocp: %s\n", sub_ocp((ocp >> 2) & 3));
// }

// !!!
// static void print_instruction_info(t_instruction *inst)
// {
// 	printf("Opcode: %s (%d)\n", op_tab[inst->opcode - 1].name, inst->opcode);
// 	// print_ocp(inst->ocp);
//
// 	printf("Param 1: %8s, %d\n", sub_ocp(inst->param_types[0]), inst->params[0]);
// 	if (inst->param_types[1])
// 		printf("Param 2: %8s, %d\n", sub_ocp(inst->param_types[1]), inst->params[1]);
// 	if (inst->param_types[2])
// 		printf("Param 3: %8s, %d\n", sub_ocp(inst->param_types[2]), inst->params[2]);
// }

static void	tab_to_param_types(char param_types[3], char arg[3])
{
	if (arg[0] == T_REG)
		param_types[0] = REG_CODE;
	else if (arg[0] == T_DIR)
		param_types[0] = DIR_CODE;
	else
		param_types[0] = IND_CODE;
	if (arg[1] == T_REG)
		param_types[1] = REG_CODE;
	else if (arg[1] == T_DIR)
		param_types[1] = DIR_CODE;
	else
		param_types[1] = IND_CODE;
	if (arg[2] == T_REG)
		param_types[2] = REG_CODE;
	else if (arg[2] == T_DIR)
		param_types[2] = DIR_CODE;
	else
		param_types[2] = IND_CODE;
}

static void	read_param(t_instruction *inst, int n,
			unsigned char ram[MEM_SIZE], int *pc)
{
	if (!inst->param_types[n])
		return ;
	if (inst->param_types[n] == REG_CODE)
	{
		inst->params[n] = ram[*pc]; // !!! Check invalid register number
		if (inst->params[n] < 1 || inst->params[n] > REG_NUMBER)
			inst->invalid = 1;
		*pc = (*pc + 1) % MEM_SIZE;
	}
	else if (inst->param_types[n] == IND_CODE || op_tab[inst->opcode - 1].d2)
	{
		inst->params[n] = two_octets_to_short(ram, *pc);
		*pc = (*pc + 2) % MEM_SIZE;
	}
	else if (inst->param_types[n] == DIR_CODE)
	{
		inst->params[n] = four_octets_to_int(ram, *pc);
		*pc = (*pc + 4) % MEM_SIZE;
	}
}

void		execute_instruction(t_proc *proc, t_vm *vm)
{
	t_instruction	inst;
	int				pc;

	inst.opcode = proc->opcode;
	if (inst.opcode < 1 || inst.opcode > 16)
	{
		proc->pc = (proc->pc + 1) % MEM_SIZE;
		pre_execute_instruction(proc, vm);
		return ;
	}
	pc = (proc->pc + 1) % MEM_SIZE;
	inst.invalid = 0;
	if (op_tab[inst.opcode - 1].ocp)
	{
		inst.ocp = vm->ram[pc];
		pc = (pc + 1) % MEM_SIZE;
		ocp_to_param_types(inst.param_types, inst.ocp);
	}
	else
		tab_to_param_types(inst.param_types, op_tab[inst.opcode - 1].arg);
	read_param(&inst, 0, vm->ram, &pc);
	read_param(&inst, 1, vm->ram, &pc);
	read_param(&inst, 2, vm->ram, &pc);
	if (!inst.invalid)
		g_op_functions[inst.opcode - 1](vm, proc, &inst);
	if (inst.opcode != 9 || inst.invalid)
		proc->pc = pc;
	pre_execute_instruction(proc, vm);
}

void	pre_execute_instruction(t_proc *proc, t_vm *vm)
{
	proc->opcode = vm->ram[proc->pc];
	proc->cycles = proc->opcode > 0 && proc->opcode <= 16 ?
		op_tab[(int)proc->opcode - 1].cycles : 1;
}
