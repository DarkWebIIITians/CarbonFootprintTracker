#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <iostream>

using namespace std;
using namespace cppcms;

// Function to calculate carbon footprint
double calculateCarbonFootprint(double publicTransportValue, double privateTransportValue, double electricityValue) {
    // Constants for carbon footprint factors
    const double publicTransportFactor = 0.02; // Carbon footprint per km for public transport
    const double privateTransportFactor = 0.05; // Carbon footprint per km for private transport
    const double electricityFactor = 0.1; // Carbon footprint per kWh for electricity

    // Calculate carbon footprint for each category
    double publicTransportFootprint = publicTransportValue * publicTransportFactor;
    double privateTransportFootprint = privateTransportValue * privateTransportFactor;
    double electricityFootprint = electricityValue * electricityFactor;

    // Total carbon footprint
    double totalFootprint = publicTransportFootprint + privateTransportFootprint + electricityFootprint;

    return totalFootprint;
}

class CarbonFootprintApp : public application {
public:
    CarbonFootprintApp(cppcms::service &srv) : application(srv) {}

    // Handler for POST requests to calculate carbon footprint
    void calculate() {
        http::request req = request();
        http::response res = response();

        double publicTransportValue = req.post("public_transport");
        double privateTransportValue = req.post("private_transport");
        double electricityValue = req.post("electricity");

        // Calculate carbon footprint
        double carbonFootprint = calculateCarbonFootprint(publicTransportValue, privateTransportValue, electricityValue);

        // Set response content type and body
        res.content_type("text/plain");
        res.set_plain_text_header();
        res.out() << "Your estimated carbon footprint is: " << carbonFootprint << " kgCO2e" << endl;
    }
};

int main(int argc, char **argv) {
    try {
        // Create CPPCMS service
        cppcms::service srv(argc, argv);

        // Create CarbonFootprintApp instance and attach URL mapper
        srv.applications_pool().mount(
            cppcms::applications_factory<CarbonFootprintApp>()
        );

        // Start the service
        srv.run();
    }
    catch (std::exception const &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
