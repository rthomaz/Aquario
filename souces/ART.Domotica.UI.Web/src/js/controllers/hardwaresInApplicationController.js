﻿'use strict';
app.controller('hardwaresInApplicationController', ['$scope', '$timeout', '$log', 'uiGridConstants', 'EventDispatcher', 'hardwaresInApplicationService', function ($scope, $timeout, $log, uiGridConstants, EventDispatcher, hardwaresInApplicationService) {    
        
    var onGetListCompleted = function (data) {
        for (var i = 0; i < data.length; i++) {
            data[i].createDateFormatted = new Date(data[i].createDate * 1000).toLocaleString();
        }
        $scope.gridOptions.data = data;
        $scope.$apply();
    }

    var onDeleteHardwareClick = function (hardware) {
        hardwaresInApplicationService.deleteHardware(hardware.id);
    }

    var onDeleteHardwareCompleted = function () {
        alert("hardware deletado!!!");
    }

    EventDispatcher.on('hardwaresInApplicationService_onGetListCompleted', onGetListCompleted);
    EventDispatcher.on('hardwaresInApplicationService_onDeleteHardwareReceived', onDeleteHardwareCompleted);

    $scope.gridOptions = {                                                 
        enableFiltering: true,
        enableSorting: true,
        showFooter: true,
        rowHeight: 36,
        data: [],
        columnDefs: [
            { name: 'Id', field: 'id', width: 270 },
            { name: 'Data criação', field: 'createDateFormatted', width: 150 },
            { name: 'Ações', cellTemplate: '<div class="text-center"><a ng-click="grid.appScope.deleteHardwareClick(row.entity)" class="btn btn-danger" href="" aria-label="Delete"><i class="fa fa-trash-o" aria-hidden="true"></i></a></div>', width: 85 },
        ],
    };

    $scope.deleteHardwareClick = onDeleteHardwareClick;

    hardwaresInApplicationService.getList();

}]);