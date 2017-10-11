/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   direct_store.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afourcad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/29 16:57:37 by afourcad          #+#    #+#             */
/*   Updated: 2017/10/11 16:18:17 by afourcad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "corewar.h"

void	direct_store(t_vm *vm, t_proc *proc, t_instruction *inst)
{
	int	value;

	value = 0;
	if (inst->param_types[1] == IND_CODE)
	{
		vm->ram[(proc->pc + (inst->params[1] % IDX_MOD)) % MEM_SIZE] =
			proc->r[inst->params[0]] & 0x000000ff;
		vm->ram[(proc->pc + 1 + (inst->params[1] % IDX_MOD)) % MEM_SIZE] =
			proc->r[inst->params[0]] & 0x0000ff00 >> 8;
		vm->ram[(proc->pc + 2 + (inst->params[1] % IDX_MOD)) % MEM_SIZE] =
			proc->r[inst->params[0]] & 0x00ff0000 >> 16;
		vm->ram[(proc->pc + 3 + (inst->params[1] % IDX_MOD)) % MEM_SIZE] =
			proc->r[inst->params[0]] & 0xff000000 >> 24;
	}
	else
		proc->r[inst->params[1]] = proc->r[inst->params[0]];
	proc->carry = inst->params[0] == 0 ? 1 : 0;
	afficher(vm);
}

void	long_direct_store(t_vm *vm, t_proc *proc, t_instruction *inst)
{
	int	value;

	value = 0;
	if (inst->param_types[1] == IND_CODE)
	{
		vm->ram[(proc->pc + inst->params[1]) % MEM_SIZE] =
			proc->r[inst->params[0]] & 0x000000ff;
		vm->ram[(proc->pc + 1 + inst->params[1]) % MEM_SIZE] =
			proc->r[inst->params[0]] & 0x0000ff00 >> 8;
		vm->ram[(proc->pc + 2 + inst->params[1]) % MEM_SIZE] =
			proc->r[inst->params[0]] & 0x00ff0000 >> 16;
		vm->ram[(proc->pc + 3 + inst->params[1]) % MEM_SIZE] =
			proc->r[inst->params[0]] & 0xff000000 >> 24;
	}
	else
		proc->r[inst->params[1]] = proc->r[inst->params[0]];
	proc->carry = inst->params[0] == 0 ? 1 : 0;
	afficher(vm);
}
