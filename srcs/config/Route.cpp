/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:41:07 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/03 09:57:00 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include <iostream>

Route::Route(JsonData & data): _data(data)
{
}

Route::Route(const Route &src): _data(src._data)
{
}

Route& Route::operator=(const Route &src)
{
	_data = src._data;
	return *this;
}

Route::~Route()
{
}
